// day03_结构体_概念演示.cpp
// 今天主线：结构体 —— 为什么 YOLOv8 的输出必须用 struct 装
//
// 编译（在 VS Code 里按 Ctrl+Shift+B 也行）：
//   g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day03_结构体_概念演示.cpp -o day03.exe
//
// 学习方式：先**猜**每段输出，再跑，再看。猜错的地方就是你今天的知识点。
//
// 注意：这个文件编译时会出 5 条 -Wmissing-field-initializers 警告，
//       是我【故意】留下的（第 1 段写法 B 只给了两个字段）。
//       目标是让你体会一件事：编译器在主动提醒你"还有 5 个成员没赋值"。
//       除了这 5 条，其余警告应该为 0。

#include <iostream>
#include <cstring>    // memset
#include <cstddef>    // offsetof
#include <cmath>      // fabs
#include <string>

// ============================================================================
// 第 1 段：定义 / 三种初始化 / 成员访问
// ============================================================================

// 这就是你毕设里要用的东西：一个"行人检测结果"
struct DetResult {
    int   cls_id;                    // 类别：0=person, 2=car ...
    float prop;                      // 置信度 0~1
    int   left, top, right, bottom;  // 框的四个边界（注意：不是 x/y/w/h）
    float depth;                     // 深度（米），-1 表示还没测出来
};

void sec1() {
    std::cout << "\n===== 第 1 段：定义 / 初始化 / 访问 =====\n";

    // 写法 A：花括号整体初始化（C++11 起最推荐，顺序必须和定义一致）
    DetResult a{0, 0.93f, 100, 200, 180, 340, 3.5f};

    // 写法 B：只给前几个，后面的自动补 0
    DetResult b{0, 0.93f};

    // 写法 C：先零初始化，再逐个赋值
    DetResult c{};
    c.cls_id = 0;
    c.prop   = 0.93f;
    c.left = 100; c.top = 200; c.right = 180; c.bottom = 340;
    c.depth = 3.5f;

    // 写法 D（反面教材）：不初始化就赋一部分
    DetResult e;          // ← 成员是【随机垃圾值】，读它就是未定义行为
    e.cls_id = 0;         //   只赋了这一个，其余 6 个还是垃圾

    std::cout << "A: cls=" << a.cls_id << " prop=" << a.prop
              << " box=(" << a.left << "," << a.top << ","
              << a.right << "," << a.bottom << ") depth=" << a.depth << "\n";
    std::cout << "B: cls=" << b.cls_id << " prop=" << b.prop
              << " box=(" << b.left << "," << b.top << ","
              << b.right << "," << b.bottom << ") depth=" << b.depth
              << "   ← 没给的字段自动为 0\n";
    std::cout << "C: 逐个赋值结果和 A 一样：left=" << c.left
              << " depth=" << c.depth << "\n";

    // 关键认知：a、b、c、e 是四个**独立**的对象，改一个不影响另一个
    a.left = -1;
    std::cout << "把 a.left 改成 -1 后，c.left 还是 " << c.left
              << "  ← 结构体是值类型，各有各的内存\n";

    std::cout << "反例 e：只赋了 cls_id=" << e.cls_id
              << "，但 e.left / e.depth 还是随机垃圾，读它们就是未定义行为\n";
}

// ============================================================================
// 第 2 段：结构体指针 与 ->
// ============================================================================

void sec2() {
    std::cout << "\n===== 第 2 段：指针 + -> =====\n";

    DetResult d{0, 0.88f, 100, 200, 180, 340, 2.0f};
    DetResult* p = &d;              // p 存放 d 的地址

    std::cout << "&d          = " << (void*)&d << "\n";
    std::cout << "p           = " << (void*)p  << "   ← 和上面是同一个地址\n";

    // 三种写法，效果完全一样，从笨到简
    std::cout << "d.left      = " << d.left      << "\n";
    std::cout << "(*p).left   = " << (*p).left   << "   ← 先解引用再用 .\n";
    std::cout << "p->left     = " << p->left     << "   ← 这就是 -> 的全部含义：(*p). 的缩写\n";

    // 用指针改，改的是本体
    p->depth = 4.2f;
    std::cout << "p->depth = 4.2 之后，d.depth = " << d.depth
              << "   ← 通过指针改，改的是本体（不是副本）\n";

    // 成员在内存里的偏移（这是 RKNN 手动解析输出 buffer 时才需要的知识）
    std::cout << "offsetof(cls_id)=" << offsetof(DetResult, cls_id)
              << " offsetof(prop)=" << offsetof(DetResult, prop)
              << " offsetof(left)=" << offsetof(DetResult, left)
              << " offsetof(depth)=" << offsetof(DetResult, depth) << "\n";
    std::cout << "sizeof(DetResult) = " << sizeof(DetResult) << " 字节\n";
}

// ============================================================================
// 第 3 段：三种传参 —— 昨天的"副本 vs 本体"在结构体上重演
// ============================================================================

void byValue(DetResult d) {          // 传值：整个结构体被【拷贝】一份
    d.left = -999;
    std::cout << "    byValue 内部：d.left 已经改成 " << d.left << " 了\n";
}
void byPtr(DetResult* p) {           // 传指针：拿到地址
    if (p == nullptr) return;        // ← 参数检查永远在解引用之前
    p->left = -999;
}
void byRef(DetResult& d) {           // 传引用：拿到别名
    d.left = -999;
}
void byConstRef(const DetResult& d) { // 只读引用：不许改
    std::cout << "    byConstRef 读到 left = " << d.left << "（只读）\n";
}

void sec3() {
    std::cout << "\n===== 第 3 段：传值 / 传指针 / 传引用 =====\n";

    DetResult a{0, 0.9f, 100, 200, 180, 340, 3.0f};
    DetResult b = a, c = a, d = a;

    byValue(a);
    std::cout << "byValue  之后 a.left = " << a.left << "   ← 没变！函数里改的是副本\n";

    byPtr(&b);
    std::cout << "byPtr    之后 b.left = " << b.left << "   ← 变了\n";

    byRef(c);
    std::cout << "byRef    之后 c.left = " << c.left << "   ← 变了\n";

    byConstRef(d);

    // 为什么有引用还要指针？两个真实理由：
    // 1) 指针可以是 nullptr（表示"没有这个结果"），引用不能为空
    // 2) 指针可以 ++ 遍历数组，引用不行
    DetResult* np = nullptr;
    byPtr(np);        // 靠函数里的 if 保住不崩
    std::cout << "传 nullptr 给 byPtr 也安全（函数里判了空）\n";
}

// ============================================================================
// 第 4 段：结构体数组 与 嵌套 —— 复原 rknn_model_zoo 的真实定义
// ============================================================================

#define OBJ_NUMB_MAX_SIZE 64

// 这两行和 yolov8.h 里的官方定义几乎一模一样，你今天先认识它，
// 下周读那个头文件时就会觉得"这不是我学过的东西吗"
typedef struct {
    int count;                          // 这一帧检测到几个目标
    DetResult results[OBJ_NUMB_MAX_SIZE];  // 最多存 64 个
} DetResultList;

void sec4() {
    std::cout << "\n===== 第 4 段：结构体数组 + 嵌套 =====\n";

    DetResultList list{};
    list.count = 3;
    list.results[0] = DetResult{0, 0.91f, 100, 200, 180, 340, 3.5f};
    list.results[1] = DetResult{0, 0.76f, 400, 210, 470, 330, 1.2f};
    list.results[2] = DetResult{2, 0.65f,  50, 300, 300, 420, 8.0f};

    std::cout << "本帧共 " << list.count << " 个目标：\n";
    for (int i = 0; i < list.count; ++i) {
        const DetResult& r = list.results[i];   // 引用，避免拷贝
        int w = r.right - r.left;
        int h = r.bottom - r.top;
        std::cout << "  #" << i << " cls=" << r.cls_id
                  << " prop=" << r.prop
                  << " 框 " << w << "x" << h
                  << " 深度 " << r.depth << " m\n";
    }

    std::cout << "sizeof(DetResultList) = " << sizeof(DetResultList)
              << " 字节 = " << sizeof(int) << " + "
              << OBJ_NUMB_MAX_SIZE << "*" << sizeof(DetResult) << "\n";

    // 结构体可以套结构体，也可以放指针
    struct StereoCam {
        std::string name;
        float fx;        // 焦距（像素）
        float baseline;  // 基线（米）
    };
    StereoCam cam{"left_cam", 700.0f, 0.12f};
    float z = cam.fx * cam.baseline / 80.0f;   // Z = f*B/d
    std::cout << "用 " << cam.name << " 算：f=" << cam.fx
              << " B=" << cam.baseline << " d=80px → Z=" << z << " m\n";
}

// ============================================================================
// 第 5 段：sizeof 与内存对齐 —— 为什么不是成员大小简单相加
// ============================================================================

struct P1 { char c; int i; };
struct P2 { int i; char c; };
struct P3 { char a; char b; char c; };
struct P4 { double d; char c; };

void sec5() {
    std::cout << "\n===== 第 5 段：sizeof 与内存对齐 =====\n";

    std::cout << "struct{char c; int i;}  sizeof = " << sizeof(P1)
              << "   （1+4=5？不对，是 8）\n";
    std::cout << "struct{int i; char c;}  sizeof = " << sizeof(P2)
              << "   （顺序换了，还是 8）\n";
    std::cout << "struct{char a,b,c;}     sizeof = " << sizeof(P3)
              << "   （3 个 char 正好 3）\n";
    std::cout << "struct{double d; char c;} sizeof = " << sizeof(P4)
              << "   （8+1=9？是 16）\n";
    std::cout << "DetResult  = " << sizeof(DetResult)
              << "   （4+4+4*4+4=28，正好没浪费）\n";

    std::cout << "结论：编译器会按最大成员宽度补齐，让每个成员都落在对齐边界上。\n";
    std::cout << "      P1 里 char 后面填了 3 个空洞，i 才能落在 offset 4。\n";
    std::cout << "这对毕设的含义：用 memcpy 往结构体里灌数据时，\n";
    std::cout << "      sizeof 必须用编译器给的，不能自己按字段加。\n";
}

// ============================================================================
// 第 6 段：清零 —— memset vs = {}，以及 memset 的两个坑
// ============================================================================

void sec6() {
    std::cout << "\n===== 第 6 段：两种清零 =====\n";

    DetResult arr[3];
    // 坑 1：memset 的长度必须是【整个数组的字节数】
    memset(arr, 0, sizeof(arr));
    std::cout << "memset 后 arr[2].left = " << arr[2].left
              << "  arr[2].depth = " << arr[2].depth
              << "   ← 全零字节：整数 0，浮点 0.0f，指针 nullptr\n";

    // 坑 2：如果 arr 已经退化成指针，sizeof(arr) 就变成 8 了！
    DetResult* q = arr;
    std::cout << "sizeof(arr) = " << sizeof(arr) << "（数组，3 个元素 * 28）"
              << "  sizeof(q) = " << sizeof(q) << "（指针，8）\n";
    std::cout << "所以在函数里收到指针时，必须额外传 n，"
              << "不能靠 sizeof 数元素个数。\n";
    // memset(q, 0, sizeof(q)) ← 这样只清了 8 个字节，剩下 28 个字节没清，是典型 bug

    // 更干净的写法：花括号零初始化，根本不用 memset
    DetResult arr2[3]{};
    std::cout << "= {} 之后 arr2[1].bottom = " << arr2[1].bottom
              << "  arr2[1].prop = " << arr2[1].prop << "\n";

    std::cout << "注意：memset 清 0 对 float 是安全的（0.0f 的全零表示就是 0）；\n";
    std::cout << "      但 memset 设成 1 就错了 —— 那得到的是约 2.37e-38 这种垃圾。\n";
    float f;
    memset(&f, 1, sizeof(f));
    std::cout << "实测 memset(&f, 1, 4) 得到 f = " << f
              << "   ← 不是 1.0！因为它是按【字节】填的。\n";
}

int main() {
    std::cout << "std::unitbuf 已开：崩溃时也能看到之前的输出\n";
    std::cout << std::unitbuf;

    sec1();
    sec2();
    sec3();
    sec4();
    sec5();
    sec6();

    std::cout << "\n===== 全部演示结束 =====\n";
    return 0;
}
