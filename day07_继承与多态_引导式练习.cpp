// ================================================================
//  day07_继承与多态_引导式练习.cpp
//  目标：把 class Device 抽象类 + 两个派生类补完，13 条断言全过
//
//  编译：g++ -std=c++17 -Wall -Wextra -Wshadow -g day07_继承与多态_引导式练习.cpp -o day07_继承与多态_引导式练习.exe
//
//  ---------------------------------------------------------------
//  【三步走，别跳】
//
//  第 1 步  直接编译 → 你会看到这条报错：
//             error: invalid new-expression of abstract class type 'CameraDevice'
//             note:  because the following virtual functions are pure within 'CameraDevice':
//             note:   virtual bool Device::open()
//           这就是【空1】的答案：CameraDevice 少实现了一个纯虚函数，
//           所以它自己也变成了抽象类，不能 new。
//
//  第 2 步  补上 空1 → 能编译能跑了，但还剩 4 条警告 + 一堆 FAIL。
//           其中 2 条长这样，是关于【空4】的，读它的原文就知道漏了什么：
//             warning: deleting object of abstract class type 'Device' which has
//                      non-virtual destructor will cause undefined behavior [-Wdelete-non-virtual-dtor]
//           另外 2 条 unused parameter 'frameId' 是在提醒你 空2、空3 还没真正用上参数。
//
//  第 3 步  边填边跑，直到 13 个 PASS、零警告
//  ---------------------------------------------------------------
//  【知识点 A】= 0 结尾的是纯虚函数。含纯虚函数的类叫抽象类：
//            不能造对象，只能有指针或引用。它就是【接口】。
//
//  【知识点 B】派生类必须实现基类【所有】纯虚函数，少一个它自己就还是抽象类。
//
//  【知识点 C】override 写在函数最后，让编译器检查你是否真的改写了基类虚函数。
//            签名必须一字不差，包括末尾的 const。
//
//  【知识点 D】有虚函数的类，析构函数必须也标 virtual。否则 delete 基类指针时
//            派生类的析构不会被调用，派生类那部分资源就泄漏了。
//            【这个错误不报错、不崩溃，只能靠断言抓。】
//
//  【知识点 E】通过基类指针调用虚函数，运行时按【对象实际类型】决定调哪个实现。
//            这就是多态：一个循环处理所有设备，代码只写一遍。
// ================================================================

#include <iostream>
#include <string>
#include <vector>

// 现在还活着的对象数（基类构造 +1、基类析构 -1；派生类构造 +1、派生类析构 -1）
// 用它来检查虚析构到底有没有把派生类析构调起来。
static int g_live = 0;

// ================================================================
//  接口：一台设备
// ================================================================
class Device {
public:
    Device() { ++g_live; }
    virtual ~Device() { --g_live; }
    virtual bool open() = 0;
    virtual bool grab(int& frameId) = 0;
    virtual const char* name() const = 0;
    int grabCount() const { return grabCount_; }
protected:
    int grabCount_ = 0;
};

class CameraDevice : public Device {
public:
    explicit CameraDevice(int total) : total_(total) { ++g_live; }
    ~CameraDevice() { --g_live; }

    bool open() override{opened_=true;return true;}

    // 【空1】在这里实现 open()：把 opened_ 置为 true 并返回 true。
    //        要写完整一行（返回值类型 + 函数名 + 空参数 + 函数体）。
    //        忘了写这一行，CameraDevice 就还是抽象类，测试里 new CameraDevice(5) 直接编不过。

    bool grab(int& frameId) override {
        // 【空2】规则：
        //   ① 没 open 过（opened_ 是 false）→ 返回 false
        //   ② 已经抓够 total_ 帧 → 返回 false
        //   ③ 否则 → frameId 取【当前已抓的帧数】（从 0 开始），grabCount_ 加 1，返回 true
        // 下面这行是空壳兜底，不是答案：它只保证程序能跑不崩，断言会 FAIL。
        if(opened_==true){
            if(grabCount_<total_){
                frameId=grabCount_;
                grabCount_++;
                return true;
            }
        }
        return false;
    }

    const char* name() const override { return "Camera"; }

private:
    int total_ = 0;
    bool opened_ = false;
};

// ================================================================
//  视频文件：抽帧读取，每抓一次跳 2 帧（毕设里降负载的常用做法）
// ================================================================
class VideoDevice : public Device {
public:
    explicit VideoDevice(int total) : total_(total) { ++g_live; }
    ~VideoDevice() { --g_live; }

    bool open() override { return true; }

    bool grab(int& frameId) override {
        // 【空3】规则（和 Camera 不一样，这里是抽帧）：
        //   这一帧的帧号 = 已抓次数 grabCount_ 乘 2（0、2、4……）
        //   算出来的帧号 >= total_ 就说明读完了，返回 false
        //   否则把帧号写进 frameId，grabCount_ 加 1，返回 true
        //   注意：视频文件 open() 永远成功，所以这里不用检查有没有 open 过
        // 下面这行是空壳兜底，不是答案：它只保证程序能跑不崩，断言会 FAIL。
        int now=grabCount_*2;
        if(now<total_){
            frameId=now;
            grabCount_++;
            return true;
        }
        return false;
    }

    const char* name() const override { return "Video"; }

private:
    int total_ = 0;
};

// ================================================================
//  测试部分：一个字都不要改，它是对的
// ================================================================
static int g_pass = 0;
static int g_fail = 0;

static void check(bool ok, const char* what) {
    if (ok) {
        std::cout << "  [PASS] " << what << std::endl;
        ++g_pass;
    } else {
        std::cout << "  [FAIL] " << what << std::endl;
        ++g_fail;
    }
}

int main() {
    std::cout << "========== 继承与多态（下）引导式练习 ==========" << std::endl;

    g_live = 0;

    Device* cam = new CameraDevice(5);
    Device* vid = new VideoDevice(6);

    check(g_live == 4, "两个对象，每个都由基类+派生类各 ++ 一次");

    check(std::string(cam->name()) == "Camera", "基类指针调到 CameraDevice::name");
    check(std::string(vid->name()) == "Video", "基类指针调到 VideoDevice::name");

    int id = -1;
    check(cam->grab(id) == false, "没 open 就抓帧，必须返回 false");
    check(cam->open() == true, "Camera open 返回 true");
    check(cam->grab(id) && id == 0, "Camera 第 1 帧 id = 0");
    check(cam->grab(id) && id == 1, "Camera 第 2 帧 id = 1");
    check(cam->grabCount() == 2, "grabCount 数得对");

    vid->open();
    check(vid->grab(id) && id == 0, "抽帧第 1 帧 id = 0");
    check(vid->grab(id) && id == 2, "抽帧第 2 帧 id = 2");
    check(vid->grab(id) && id == 4, "抽帧第 3 帧 id = 4");
    check(vid->grab(id) == false, "抽到 6 帧之外，返回 false");

    delete cam;
    delete vid;
    check(g_live == 0, "虚析构：删基类指针时派生类析构也跑了，没有泄漏");

    std::cout << "  通过 " << g_pass << " / 失败 " << g_fail << std::endl;
    return g_fail == 0 ? 0 : 1;
}
