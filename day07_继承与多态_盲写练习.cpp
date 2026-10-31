// ============================================================================
//  day07_继承与多态_盲写练习.cpp
//
//  目标：30 条断言全绿，并且【零警告】
//
//  编译（VSCode 里直接 Ctrl+Shift+B，参数已经配好）：
//    g++ -std=c++17 -Wall -Wextra -Wshadow -g 本文件.cpp -o 本文件.exe
//
//  ★ 第一次编译会出现【7 条警告】，每条都对应一个还没填的空，全部正常：
//      3 条 unused parameter 'frameId'  -> 空4、空6、空7
//      2 条 unused parameter 'ds' / 'n' -> 空8
//      2 条 deleting object of abstract class type 'Device' which has
//           non-virtual destructor       -> 空1，这条最值钱
//    警告全部消失，就说明每个空都真的用上了。
//
//  ★ 基线是【6 / 30】，那不是你的成绩，是空模板的底。
//
//  ★ 8 个空：
//      空1  基类析构少一个关键字
//      空2  frameCount 的函数体
//      空3  CameraDevice::open 的函数体
//      空4  CameraDevice::grab 的函数体
//      空5  三个派生类的 name()（三处，各返回一个中文名）
//      空6  VideoDevice::grab 的函数体
//      空7  ImageSeqDevice::grab 的函数体
//      空8  runAll 的函数体
// ============================================================================

#include <iostream>
#include <string>

// ---------------------------------------------------------------------------
//  测试框架（已给，不用改）
// ---------------------------------------------------------------------------
static int g_pass = 0;
static int g_fail = 0;

static void check(bool ok, const char* what) {
    if (ok) {
        ++g_pass;
        std::cout << "  [PASS] " << what << "\n";
    } else {
        ++g_fail;
        std::cout << "  [FAIL] " << what << "\n";
    }
}

// 存活计数：构造函数 +1，析构函数 -1。第 6 题靠它判断虚析构有没有生效。
static int g_camLive = 0;
static int g_vidLive = 0;
static int g_imgLive = 0;

// ============================================================================
//  【题目 1】抽象基类 Device —— 三个纯虚函数已经给你了
// ============================================================================
class Device {
public:
    // 【空1】这里现在少一个关键字。加上它之后：
    //        - 编译时那条 delete-non-virtual-dtor 警告会消失
    //        - 第 6 题才能过（通过基类指针 delete 时，派生类析构才会被调用）
    virtual ~Device() {}

    // 三个纯虚函数 = 接口契约，派生类必须全部实现，否则那类不能被 new
    virtual bool open() = 0;
    virtual bool grab(int& frameId) = 0;
    virtual const char* name() const = 0;

    // 【空2】非虚成员函数：返回已经抓了多少帧
    //        提示：成员就叫 grabCount_
    int frameCount() const {
        return grabCount_;                          // ← 改这里
    }

protected:
    Device() {}                            // 保护构造：外面不能直接造 Device
    int grabCount_ = 0;
};

// ============================================================================
//  【题目 2】CameraDevice —— 连续帧：0, 1, 2, ...
// ============================================================================
class CameraDevice : public Device {
public:
    explicit CameraDevice(int total) : total_(total) { ++g_camLive; }
    ~CameraDevice() { --g_camLive; }       // 已给，不用改

    // 【空3】打开设备：把 opened_ 置为 true，返回 true
    //        注意：不能只 return true，否则设备状态还是"没打开"
    bool open() override {
        opened_=true;
        return true;                      // ← 改这里
    }

    // 【空4】抓一帧
    //   ① 没 open 过 → 返回 false
    //   ② 已经抓够 total_ 帧 → 返回 false
    //   ③ 否则 → frameId = grabCount_，grabCount_ 加 1，返回 true
    bool grab(int& frameId) override {
        if(opened_==true){
            if(grabCount_<total_){
                frameId=grabCount_;
                grabCount_++;
                return true;
            }
        }
        return false;                      // ← 改这里
    }

    // 【空5-1】返回 "摄像头"
    const char* name() const override {
        return "摄像头";                        // ← 改这里
    }

private:
    int total_ = 0;
    bool opened_ = false;
};

// ============================================================================
//  【题目 3】VideoDevice —— 抽帧：0, 2, 4, ...
// ============================================================================
class VideoDevice : public Device {
public:
    explicit VideoDevice(int total) : total_(total) { ++g_vidLive; }
    ~VideoDevice() { --g_vidLive; }        // 已给

    // 视频文件"打开"永远成功，所以这个已经写好了
    bool open() override { return true; }

    // 【空6】抓一帧，但是【抽帧】：每抓一次跳 2 帧
    //   ① 下一个帧号 = grabCount_ * 2
    //   ② 如果它 >= total_ → 返回 false
    //   ③ 否则 frameId = 这个帧号，grabCount_ 加 1，返回 true
    //   注意：视频文件不用检查有没有 open 过
    bool grab(int& frameId) override {
        int now=grabCount_*2;
        if(now<total_){
            frameId=now;
            grabCount_++;
            return true;
        }
        return false;                      // ← 改这里
    }

    // 【空5-2】返回 "视频文件"
    const char* name() const override {
        return "视频文件";                        // ← 改这里
    }

private:
    int total_ = 0;
};

// ============================================================================
//  【题目 4】ImageSeqDevice —— 第三种来源，用来证明"加一种设备不用改上层"
// ============================================================================
class ImageSeqDevice : public Device {
public:
    explicit ImageSeqDevice(int total) : total_(total) { ++g_imgLive; }
    ~ImageSeqDevice() { --g_imgLive; }     // 已给

    bool open() override { return true; }  // 图片序列也不需要 open

    // 【空7】抓一帧，步骤变成 3：0, 3, 6, ...
    //   规则和 VideoDevice 一样，只是乘 3 不是乘 2
    bool grab(int& frameId) override {
        int now=grabCount_*3;
        if(now<total_){
            frameId=now;
            grabCount_++;
            return true;
        }
        return false;                      // ← 改这里
    }

    // 【空5-3】返回 "图片序列"
    const char* name() const override {
        return "图片序列";                        // ← 改这里
    }
    
private:
    int total_ = 0;
};

// ============================================================================
//  【题目 5】统一的调度函数
// ============================================================================
//  这是整个多态的意义所在：上层只写这一份代码，
//  以后加第四种、第五种设备，这个函数一个字都不用改。
//
// 【空8】函数体：
//   对每个设备：
//     - 打印 "  [名字]"，名字从 ds[i]->name() 取（这正是多态）
//     - 反复调 grab 直到它返回 false，数一下取到几帧
//     - 打印 " 取到 N 帧"
//     - 把 N 累加到 total
//   最后返回 total
int runAll(Device* ds[], int n) {
    int total_=0;
    int frameId=-1;
    for(int i=0;i<n;i++){
        int cn=0;
        while(ds[i]->grab(frameId)){
            cn++;
        }
        std::cout<<"["<<ds[i]->name()<<"] "<<"取到第"<<cn<<"帧"<<std::endl;
        total_+=cn;
    }
    return total_;                              // ← 改这里
}

// ============================================================================
//  main —— 断言部分，一个字都不要改
// ============================================================================
int main() {
    // ------------------------------------------------------------------
    std::cout << "==============================================\n";
    std::cout << "  第 1 题  基类 Device（接口与成员函数）\n";
    std::cout << "==============================================\n";
    {
        CameraDevice cam(3);

        // 空2：frameCount 读的是成员 grabCount_，刚造出来应该是 0
        check(cam.frameCount() == 0, "刚造出来 frameCount() = 0");

        cam.open();
        int id = -1;
        cam.grab(id);
        check(cam.frameCount() == 1, "取了 1 帧之后 frameCount() = 1");

        // frameCount 是【非虚】成员函数，通过基类指针调也得到同一个结果
        Device* pd = &cam;
        check(pd->frameCount() == 1, "通过基类指针调 frameCount() 结果相同");
    }
    // ------------------------------------------------------------------
    std::cout << "==============================================\n";
    std::cout << "  第 2 题  CameraDevice（连续帧 0,1,2）\n";
    std::cout << "==============================================\n";
    {
        CameraDevice cam(3);
        int id = -1;

        check(cam.grab(id) == false, "没 open 过，grab 应该失败");
        check(cam.open() == true, "open 返回 true");

        bool ok = cam.grab(id);
        check(ok && id == 0, "第 1 帧 id = 0");
        ok = cam.grab(id);
        check(ok && id == 1, "第 2 帧 id = 1");
        ok = cam.grab(id);
        check(ok && id == 2, "第 3 帧 id = 2");
        check(cam.grab(id) == false, "只有 3 帧，第 4 次应该失败");
        check(cam.frameCount() == 3, "frameCount 数得对");

        check(std::string(cam.name()) == "摄像头", "name() 是 摄像头");

        // 通过基类指针调 name()，验证多态真的生效
        Device* pd = &cam;
        check(std::string(pd->name()) == "摄像头", "基类指针调 name() 也是 摄像头");
    }

    // ------------------------------------------------------------------
    std::cout << "\n==============================================\n";
    std::cout << "  第 3 题  VideoDevice（抽帧 0,2,4）\n";
    std::cout << "==============================================\n";
    {
        VideoDevice vid(6);            // 总共 6 帧，抽出来是 0,2,4
        int id = -1;

        vid.open();
        bool ok = vid.grab(id);
        check(ok && id == 0, "第 1 帧 id = 0");
        ok = vid.grab(id);
        check(ok && id == 2, "第 2 帧 id = 2（跳过了 1）");
        ok = vid.grab(id);
        check(ok && id == 4, "第 3 帧 id = 4（跳过了 3）");
        check(vid.grab(id) == false, "下一个是 6，超出总数，应该失败");
        check(vid.frameCount() == 3, "frameCount 数得对");
        check(std::string(vid.name()) == "视频文件", "name() 是 视频文件");
    }

    // ------------------------------------------------------------------
    std::cout << "\n==============================================\n";
    std::cout << "  第 4 题  ImageSeqDevice（步长 3：0,3,6）\n";
    std::cout << "==============================================\n";
    {
        ImageSeqDevice img(9);
        int id = -1;

        img.open();
        bool ok = img.grab(id);
        check(ok && id == 0, "第 1 帧 id = 0");
        ok = img.grab(id);
        check(ok && id == 3, "第 2 帧 id = 3");
        ok = img.grab(id);
        check(ok && id == 6, "第 3 帧 id = 6");
        check(img.grab(id) == false, "下一个是 9，超出总数，应该失败");
        check(std::string(img.name()) == "图片序列", "name() 是 图片序列");
    }

    // ------------------------------------------------------------------
    std::cout << "\n==============================================\n";
    std::cout << "  第 5 题  一个循环调度三种设备\n";
    std::cout << "==============================================\n";
    {
        CameraDevice cam(3);
        VideoDevice vid(6);
        ImageSeqDevice img(9);
        cam.open();
        vid.open();
        img.open();

        Device* ds[3] = { &cam, &vid, &img };
        int total = runAll(ds, 3);

        check(total == 9, "三个设备一共取到 9 帧");
        check(cam.frameCount() == 3, "跑完之后摄像头计数 = 3");
        check(vid.frameCount() == 3, "跑完之后视频计数 = 3");
        check(img.frameCount() == 3, "跑完之后图片序列计数 = 3");
    }

    // ------------------------------------------------------------------
    std::cout << "\n==============================================\n";
    std::cout << "  第 6 题  虚析构：通过基类指针 delete\n";
    std::cout << "==============================================\n";
    {
        int before = g_camLive;

        Device* p = new CameraDevice(3);
        check(g_camLive == before + 1, "new 之后存活数 +1");

        delete p;                       // 关键在这里：p 的静态类型是 Device*
        check(g_camLive == before, "delete 之后派生类析构被调用了（虚析构生效）");

        int before2 = g_vidLive;
        Device* q = new VideoDevice(6);
        delete q;
        check(g_vidLive == before2, "VideoDevice 也一样（虚析构生效）");
    }

    // ------------------------------------------------------------------
    std::cout << "\n==============================================\n";
    std::cout << "  通过 " << g_pass << " / 失败 " << g_fail
              << "    共 " << (g_pass + g_fail) << " 条\n";
    std::cout << "==============================================\n";
    return 0;
}
