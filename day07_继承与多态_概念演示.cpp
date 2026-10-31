// ================================================================
//  day07_继承与多态_概念演示.cpp
//  类与对象（下）：继承、构造析构顺序、虚函数、多态、纯虚函数、虚析构
//
//  编译：
//    g++ -std=c++17 -Wall -Wextra -Wshadow -g day07_继承与多态_概念演示.cpp -o day07_继承与多态_概念演示.exe
//
//  ---------------------------------------------------------------
//  【有两段代码我故意没写进文件，因为它们编译不过】
//  你只会在引导式练习的第 1 步亲眼看到这两类报错。先在这里记住样子：
//
//  ① 抽象类不能造对象
//        IDevice d;
//      error: cannot declare variable 'd' to be of abstract type 'IDevice'
//      note:  because the following virtual functions are pure within 'IDevice':
//      note:    virtual bool IDevice::open()
//
//     note 直接告诉你【是哪个纯虚函数没被实现】，报错不用猜。
//
//  ② override 抓笔误
//        class Cam : public IDevice {
//            bool opne() override { return true; }   // 拼错成 opne
//        };
//      error: 'virtual bool Cam::opne()' marked 'override', but does not override
//
//     没有 override 的话，这个笔误会【安静地】变成一个新函数，
//     基类的 open() 依然是纯虚的，你要多绕很多弯才查得出来。
//  ---------------------------------------------------------------
//
//  预期：0 个警告。
// ================================================================

#include <iostream>
#include <string>
#include <vector>

// ================================================================
//  第 1 节：继承的基本形态 —— 三种权限在派生类里各是什么待遇
// ================================================================
struct Base {
public:
    int pub = 1;
protected:
    int prot = 2;
private:
    int priv = 3;
};

struct PubDerived : public Base {
    void look() {
        std::cout << "    【派生类内部】pub 能碰 = " << pub
                  << "，prot 能碰 = " << prot
                  << "，priv 碰不到（private 只给本类自己用）" << std::endl;
        // 如果你在这里写 std::cout << priv; 会报：
        //   error: 'int Base::priv' is private within this context
    }
};

// struct 默认是 public 继承，class 默认是 private 继承 —— 今天第一个坑
class PriDerived : Base {          // 等价于 class PriDerived : private Base
public:
    void look() {
        pub = 99;                  // 能编过：private 继承后 pub 降到本类的 private
        std::cout << "    【private 继承】派生类内部照样能改 pub，改成 " << pub << std::endl;
    }
};

// ================================================================
//  第 2 节：构造与析构的顺序 —— 基类必须先构造
// ================================================================
struct LayerA {
    explicit LayerA(int v) {
        std::cout << "    LayerA 构造，v = " << v << std::endl;
    }
    ~LayerA() {
        std::cout << "    LayerA 析构" << std::endl;
    }
};

struct LayerB : LayerA {
    // LayerA 只有 LayerA(int)，没有默认构造，所以这里【必须】显式写出 : LayerA(100)。
    // 少写就报：error: no matching function for call to 'LayerA::LayerA()'
    LayerB() : LayerA(100) {
        std::cout << "    LayerB 构造（LayerA 的构造已经先跑完了）" << std::endl;
    }
    ~LayerB() {
        std::cout << "    LayerB 析构" << std::endl;
    }
};

// ================================================================
//  第 3 节：没有 virtual —— 静态绑定，调到的不是你想要的
// ================================================================
struct DevBase {
    void who() {
        std::cout << "    DevBase::who()   ← 我不想调这个" << std::endl;
    }
};

struct DevCam : DevBase {
    void who() {
        std::cout << "    DevCam::who()    ← 我要的是这个" << std::endl;
    }
};

// ================================================================
//  第 4 节：加 virtual —— 动态绑定，才叫多态
// ================================================================
struct VBase {
    virtual void who() const {
        std::cout << "    VBase::who()" << std::endl;
    }
    virtual ~VBase() = default;      // 有虚函数的类，析构必须也是虚的（第 6 节讲）
};

struct VCam : VBase {
    // override 写在最后：告诉编译器【我是在改写基类的虚函数】
    // 签名必须一字不差，包括末尾的 const
    void who() const override {
        std::cout << "    VCam::who()" << std::endl;
    }
};

// ================================================================
//  第 5 节：纯虚函数 = 抽象类 = 只定义接口，不提供实现
// ================================================================
struct IDevice {
    virtual ~IDevice() = default;

    // 结尾的 = 0 就是【纯虚函数】。含纯虚函数的类是抽象类，不能造对象，只能有指针。
    virtual bool open() = 0;
    virtual const char* name() const = 0;

    // 不是纯虚的虚函数可以带默认实现，派生类愿意覆盖就覆盖，不愿意就用这个
    virtual int frameCount() const { return 0; }
};

struct CamDev : IDevice {
    bool open() override { return true; }
    const char* name() const override { return "Camera"; }
    int frameCount() const override { return 300; }
};

struct ImgSeqDev : IDevice {
    bool open() override { return true; }
    const char* name() const override { return "ImageSeq"; }
    // 故意不覆盖 frameCount，用基类的默认 0
};

// ================================================================
//  第 6 节：虚析构 —— 不写它不会报错，只会静默泄漏
// ================================================================
struct NoVirtDtor {
    ~NoVirtDtor() {
        std::cout << "        NoVirtDtor 析构" << std::endl;
    }
};

struct DerNoVirt : NoVirtDtor {
    ~DerNoVirt() {
        std::cout << "        DerNoVirt 析构" << std::endl;
    }
};

struct HasVirtDtor {
    virtual ~HasVirtDtor() {
        std::cout << "        HasVirtDtor 析构" << std::endl;
    }
};

struct DerHasVirt : HasVirtDtor {
    ~DerHasVirt() {
        std::cout << "        DerHasVirt 析构" << std::endl;
    }
};

// ================================================================
int main() {
    std::cout << "==============================================" << std::endl;
    std::cout << "  类与对象（下）：继承与多态" << std::endl;
    std::cout << "==============================================" << std::endl;

    // ---------------- 第 1 节 ----------------
    std::cout << std::endl << "===== 第 1 节：继承和三种访问权限 =====" << std::endl;
    {
        PubDerived pd;
        pd.pub = 10;
        std::cout << "    public 继承：外部能直接改 pd.pub，现在是 " << pd.pub << std::endl;
        pd.look();

        PriDerived pr;
        pr.look();
        // 下面这行编译不过，private 继承把 pub 藏起来了：
        //   pr.pub = 1;
        //   error: 'int Base::pub' is inaccessible
        std::cout << "    private 继承：外部写 pr.pub 直接报 inaccessible" << std::endl;
    }

    // ---------------- 第 2 节 ----------------
    std::cout << std::endl << "===== 第 2 节：构造析构顺序 =====" << std::endl;
    {
        std::cout << "    —— 进入作用域 ——" << std::endl;
        LayerB b;
        std::cout << "    —— 离开作用域 ——" << std::endl;
    }
    std::cout << "    结论：构造是基类先、派生后；析构正好反过来，派生先、基类后。" << std::endl;

    // ---------------- 第 3 节 ----------------
    std::cout << std::endl << "===== 第 3 节：没有 virtual 会怎样 =====" << std::endl;
    {
        DevCam cam;
        DevCam* pc = &cam;
        DevBase* pb = &cam;       // 同一个对象，换个指针类型看

        std::cout << "    用 DevCam* 调：" << std::endl;
        pc->who();
        std::cout << "    用 DevBase* 调（这才是日常写法）：" << std::endl;
        pb->who();
        std::cout << "    明明指着 DevCam 对象，却调了基类版本。" << std::endl;
        std::cout << "    因为编译器只看指针的【静态类型】DevBase*，编译期就把函数地址钉死了。" << std::endl;
    }

    // ---------------- 第 4 节 ----------------
    std::cout << std::endl << "===== 第 4 节：加 virtual，才叫多态 =====" << std::endl;
    {
        VCam vcam;
        VBase* vpb = &vcam;
        std::cout << "    用 VBase* 调：" << std::endl;
        vpb->who();
        std::cout << "    这次真的走到 VCam 的实现了。函数地址是运行时按对象实际类型查的。" << std::endl;

        std::vector<VBase*> list;
        list.push_back(&vcam);
        std::cout << "    放进 vector 再调，效果一样：" << std::endl;
        for (VBase* p : list) {
            p->who();
        }

        VBase sliced = vcam;      // 切片：只把对象里【基类那部分】复制过来
        std::cout << "    按值复制一份再调（切片）：" << std::endl;
        sliced.who();
        std::cout << "    所以多态【只能通过指针或引用】。参数写成 VBase v 就会切片，" << std::endl;
        std::cout << "    必须写 VBase& v 或 VBase* v。" << std::endl;
    }

    // ---------------- 第 5 节 ----------------
    std::cout << std::endl << "===== 第 5 节：纯虚函数与抽象类 =====" << std::endl;
    {
        std::vector<IDevice*> devs;
        devs.push_back(new CamDev());
        devs.push_back(new ImgSeqDev());

        std::cout << "    一个循环，两种设备，代码只写一遍：" << std::endl;
        for (IDevice* d : devs) {
            d->open();
            std::cout << "      " << d->name()
                      << "，frameCount = " << d->frameCount() << std::endl;
        }

        for (IDevice* d : devs) {
            delete d;             // 这里能安全 delete，靠的就是 IDevice 的虚析构
        }
    }

    // ---------------- 第 6 节 ----------------
    std::cout << std::endl << "===== 第 6 节：虚析构 =====" << std::endl;
    {
        std::cout << "    【没有 virtual 析构】delete 基类指针：" << std::endl;
        NoVirtDtor* p1 = new DerNoVirt();
        delete p1;
        std::cout << "      ↑ 只跑了基类的析构。派生类那部分没被清理，这叫泄漏。" << std::endl;

        std::cout << "    【有 virtual 析构】delete 基类指针：" << std::endl;
        HasVirtDtor* p2 = new DerHasVirt();
        delete p2;
        std::cout << "      ↑ 派生类先析构、基类后析构，顺序和栈上对象一致。" << std::endl;

        std::cout << "    这一节没有任何报错、没有任何警告。这就是它最贵的地方：" << std::endl;
        std::cout << "    你只是悄悄泄漏了内存，程序照跑，直到跑够久才崩。" << std::endl;
    }

    std::cout << std::endl << "============== 演示结束 ==============" << std::endl;
    return 0;
}
