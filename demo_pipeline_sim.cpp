// ============================================================
// demo_pipeline_sim.cpp
// 目的：把 rknn_model_zoo 里 yolov8 例程的主流程，用你已学过的 C++
//       原样走一遍。跑完你就知道那个 demo 到底在干什么。
//
// 不需要：板子 / 网络 / OpenCV / NPU / 虚拟机
// 编译：g++ -std=c++17 -Wall -Wextra -Wshadow demo_pipeline_sim.cpp -o demo_pipeline_sim.exe
//
// 对应官方源码位置：
//   第 0 节 -> examples/yolov8/cpp/yolov8.h
//   第 1 节 -> main.cc 的 init_model()   -> rknn_init
//   第 2 节 -> main.cc 的 LetterBox()    -> 前处理
//   第 3 节 -> main.cc 的 rknn_run()     -> 推理
//   第 4 节 -> postprocess.cc            -> 解码 + NMS
//   第 5 节 -> utils/imagedrawing        -> 画框
// ============================================================

#include <iostream>
#include <vector>
#include <string>

// ------------------------------------------------------------
// 第 0 节  数据结构
// 官方 yolov8.h 里是 object_detect_result { int cls_id; float prop; box_rect_t bbox; }
// 我们用 struct 套 struct 表达同一个东西
// ------------------------------------------------------------
struct Box {
    int left;
    int top;
    int right;
    int bottom;
};

struct DetectResult {
    int   cls_id;
    float prop;
    Box   box;
};

// ------------------------------------------------------------
// 第 1 节  加载模型   -> 对应 rknn_init()
// ------------------------------------------------------------
struct Model {
    std::string path;
    int  input_w;
    int  input_h;
    bool ready;
};

Model load_model(const std::string& path)
{
    Model m;
    m.path    = path;
    m.input_w = 640;
    m.input_h = 640;
    m.ready   = true;

    std::cout << "【1】加载模型 " << m.path << "\n";
    std::cout << "     调 rknn_init，返回 0 表示成功，模型可用\n";
    std::cout << "     模型要求输入: " << m.input_w << " x " << m.input_h << " x 3\n";
    return m;
}

// ------------------------------------------------------------
// 第 2 节  前处理 letterbox   -> 对应 main.cc 的 LetterBox()
// 为什么不能直接 resize 到 640x640：
//   直接 resize 会把长宽比拉歪，行人被拉成胖子，检测精度掉
//   letterbox = 等比缩放 + 两侧补灰边，长宽比不变
// ------------------------------------------------------------
struct Preprocessed {
    int   width;
    int   height;
    float scale;      // 缩放比，后处理要把框坐标除以它还原
    int   pad_x;      // 左右各补多少像素
    int   pad_y;      // 上下各补多少像素
};

Preprocessed letterbox(int src_w, int src_h, int dst_w, int dst_h)
{
    float sx = (float)dst_w / (float)src_w;
    float sy = (float)dst_h / (float)src_h;
    float s  = sx < sy ? sx : sy;        // 取小的，保证整张图塞得进去

    Preprocessed p;
    p.width  = dst_w;
    p.height = dst_h;
    p.scale  = s;
    p.pad_x  = (dst_w - (int)((float)src_w * s)) / 2;
    p.pad_y  = (dst_h - (int)((float)src_h * s)) / 2;
    return p;
}

// ------------------------------------------------------------
// 第 3 节  推理   -> 对应 rknn_run()
// YOLOv8 输出 8400 个候选框，这个数字是这么来的：
//   80x80 + 40x40 + 20x20 = 6400 + 1600 + 400 = 8400
// 三个尺度分别负责小 / 中 / 大目标。记住 8400，以后看日志对不上就是这里错了。
// ------------------------------------------------------------
const int kNumCandidates = 8400;

int run_inference(const Model& m)
{
    std::cout << "【3】rknn_run 推理一次\n";
    std::cout << "     输入: 1 x " << m.input_w << " x " << m.input_h << " x 3\n";
    std::cout << "     输出: 1 x " << kNumCandidates << " x 84\n";
    std::cout << "     8400 = 80*80 + 40*40 + 20*20，三个尺度负责小中大目标\n";
    std::cout << "     84  = 4 个框坐标 + 80 个类别的分数\n";
    return kNumCandidates;
}

// ------------------------------------------------------------
// 第 4 节  后处理   -> 对应 postprocess.cc
// 8400 个候选里有大量重复框（同一个人被周围好几个格子都报了一遍），
// 必须用 NMS（非极大值抑制）去重。这是后处理的核心，也是最难的一段。
// ------------------------------------------------------------

// 两个框的重叠程度，0 表示不重叠，1 表示完全重合
float iou(const Box& a, const Box& b)
{
    int ix1 = a.left   > b.left   ? a.left   : b.left;
    int iy1 = a.top    > b.top    ? a.top    : b.top;
    int ix2 = a.right  < b.right  ? a.right  : b.right;
    int iy2 = a.bottom < b.bottom ? a.bottom : b.bottom;
    if (ix2 <= ix1 || iy2 <= iy1) return 0.0f;

    float inter  = (float)((ix2 - ix1) * (iy2 - iy1));
    float area_a = (float)((a.right - a.left) * (a.bottom - a.top));
    float area_b = (float)((b.right - b.left) * (b.bottom - b.top));
    return inter / (area_a + area_b - inter);
}

std::vector<DetectResult> nms(std::vector<DetectResult> cands, float thr)
{
    // 第一步：按置信度从高到低排（置信度高的优先保留）
    for (std::size_t i = 0; i < cands.size(); ++i) {
        for (std::size_t j = i + 1; j < cands.size(); ++j) {
            if (cands[j].prop > cands[i].prop) {
                DetectResult t = cands[i];
                cands[i] = cands[j];
                cands[j] = t;
            }
        }
    }

    // 第二步：逐个看，跟已保留的框重叠太多就丢掉
    std::vector<DetectResult> keep;
    for (std::size_t i = 0; i < cands.size(); ++i) {
        bool drop = false;
        for (std::size_t k = 0; k < keep.size(); ++k) {
            if (iou(cands[i].box, keep[k].box) > thr) {
                drop = true;
                break;
            }
        }
        if (!drop) keep.push_back(cands[i]);
    }
    return keep;
}

// ------------------------------------------------------------
// 第 5 节  画框   -> 对应 utils/imagedrawing
// 真程序里这里调的是 imagedrawing 的 rectangle / putText，
// 我们这里用字符画个示意，让你看见框长什么样
// ------------------------------------------------------------
void draw_boxes(const std::vector<DetectResult>& res, int img_w, int img_h)
{
    std::cout << "【5】画框并保存 out.jpg\n";
    for (std::size_t i = 0; i < res.size(); ++i) {
        const DetectResult& r = res[i];
        float cx = (float)(r.box.left + r.box.right) * 0.5f;
        float cy = (float)(r.box.top + r.box.bottom) * 0.5f;
        std::cout << "     框" << (i + 1)
                  << " class_id=" << r.cls_id
                  << " 置信度=" << r.prop
                  << " 坐标[" << r.box.left << "," << r.box.top
                  << "," << r.box.right << "," << r.box.bottom << "]\n";
        std::cout << "          框中心在图上的位置: 横向 "
                  << (int)(cx / (float)img_w * 100.0f) << "%，纵向 "
                  << (int)(cy / (float)img_h * 100.0f) << "%\n";
    }
}

int main()
{
    std::cout << "========== 模拟 rknn_yolov8_demo 主流程 ==========\n\n";

    const std::string model_path = "model/yolov8n.rknn";
    const std::string image_path = "model/bus.jpg";
    const int src_w = 1080;
    const int src_h = 810;

    // 第 1 节
    Model m = load_model(model_path);

    // 第 2 节
    std::cout << "\n【2】读图 " << image_path << "，原始尺寸 " << src_w << " x " << src_h << "\n";
    Preprocessed p = letterbox(src_w, src_h, m.input_w, m.input_h);
    std::cout << "     letterbox 缩放比 = " << p.scale << "\n";
    std::cout << "     缩放后变成 " << (int)((float)src_w * p.scale)
              << " x " << (int)((float)src_h * p.scale) << "\n";
    std::cout << "     左右各补 " << p.pad_x << " 像素，上下各补 " << p.pad_y << " 像素\n";
    std::cout << "     这一步还要做归一化：像素 0-255 除以 255 变成 0-1\n";

    // 第 3 节
    int cand = run_inference(m);

    // 第 4 节
    std::cout << "\n【4】后处理：从 " << cand << " 个候选里筛\n";
    std::cout << "     真实程序里这 8400 条是从 rknn_outputs_get 拿到的，\n";
    std::cout << "     下面造 5 条代表性数据（真实场景的缩影）\n";

    std::vector<DetectResult> cands;
    cands.push_back({0, 0.89f, {52, 400, 250, 780}});    // 人 cls_id=0
    cands.push_back({0, 0.72f, {58, 398, 254, 782}});    // 跟上面几乎是同一个框
    cands.push_back({5, 0.91f, {20, 220, 800, 720}});    // 公交车 cls_id=5
    cands.push_back({0, 0.65f, {800, 380, 960, 700}});   // 另一个人
    cands.push_back({0, 0.41f, {54, 402, 248, 776}});    // 也是重复

    std::cout << "     NMS 前: " << cands.size() << " 条\n";
    std::vector<DetectResult> res = nms(cands, 0.45f);
    std::cout << "     NMS 后: " << res.size() << " 条（重复框被丢掉）\n";

    // 毕设关键：只要行人
    std::cout << "\n【★】毕设只要行人，所以再过滤一次 class_id == 0\n";
    std::cout << "     COCO 数据集第 0 类就是 person，这是白送的\n";
    std::vector<DetectResult> persons;
    for (std::size_t i = 0; i < res.size(); ++i) {
        if (res[i].cls_id == 0) persons.push_back(res[i]);
    }
    std::cout << "     过滤后剩 " << persons.size() << " 个人\n\n";

    // 第 5 节
    draw_boxes(persons, src_w, src_h);

    std::cout << "\n========== 结束 ==========\n";
    std::cout << "整个 demo 就这五步。你在板上跑的那条命令：\n";
    std::cout << "  ./rknn_yolov8_demo model/yolov8n.rknn model/bus.jpg out.jpg\n";
    std::cout << "参数依次是 模型路径 / 输入图片 / 输出图片，没别的。\n";
    return 0;
}
