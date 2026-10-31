// ============================================================================
//  day07b_多态_为什么需要.cpp
//  目的：同一个场景写三遍，看清 virtual 到底解决了什么问题
//
//  场景：程序要处理两种输入源——摄像头（连续帧）和视频文件（抽帧）
//        上层只想写【一个循环】，把两种来源都跑一遍
//
//  看的时候不要管语法，只盯着每一节的【输出】对比：
//    第 1 节：写了两份几乎一样的代码
//    第 2 节：只写一份了，但结果全错，而且不报错
//    第 3 节：加一个关键字，结果对了
//    第 4 节：这个关键字的代价，能用 sizeof 量出来
// ============================================================================

#include <iostream>

// ============================================================================
//  第 1 节：不用继承，各写一套
// ============================================================================
//  摄像头和视频文件是两个毫无关系的类。
//  上层想跑它们，只能写两个函数——注意 runCamera 和 runVideo 几乎一模一样。
//  每加一种输入源（比如图片序列），就要再写第三个几乎一样的函数。

struct Camera1 {
    int next_ = 0;
    bool get(int& frameId) {
        if (next_ >= 3) return false;   // 摄像头：连续 0,1,2
        frameId = next_++;
        return true;
    }
    const char* name() const { return "摄像头"; }
};

struct Video1 {
    int next_ = 0;
    bool get(int& frameId) {
        int id = next_ * 2;             // 视频文件：抽帧 0,2,4
        if (id >= 6) return false;
        next_++;
        frameId = id;
        return true;
    }
    const char* name() const { return "视频文件"; }
};

void runCamera(Camera1& c) {
    int id = -1;
    while (c.get(id)) {
        std::cout << "    [" << c.name() << "] 取到第 " << id << " 帧\n";
    }
}

void runVideo(Video1& v) {
    int id = -1;
    while (v.get(id)) {                  // 这段和上面那段的逻辑完全相同
        std::cout << "    [" << v.name() << "] 取到第 " << id << " 帧\n";
    }
}

// ============================================================================
//  第 2 节：用继承，但【忘了写 virtual】
// ============================================================================
//  现在 Camera2 和 Video2 都继承 Device2，上层可以只写一个循环来处理两种设备。
//  看起来很美——但输出是错的。仔细看：名字显示的是"基类"，帧号也是基类的规则。
//
//  为什么会这样？因为 ds[i] 的类型是 Device2*，
//  编译器在【编译时】就按 Device2 去找 get()，它根本不知道运行时指向谁。

struct Device2 {
    int next_ = 0;
    bool get(int& frameId) {             // ← 少了一个 virtual
        if (next_ >= 5) return false;    // 基类的规则：连续 0..4
        frameId = next_++;
        return true;
    }
    const char* name() const { return "Device2(基类)"; }
};

struct Camera2 : Device2 {
    bool get(int& frameId) {             // 这个函数写了，但没人调它
        if (next_ >= 3) return false;
        frameId = next_++;
        return true;
    }
    const char* name() const { return "摄像头"; }
};

struct Video2 : Device2 {
    bool get(int& frameId) {
        int id = next_ * 2;
        if (id >= 6) return false;
        next_++;
        frameId = id;
        return true;
    }
    const char* name() const { return "视频文件"; }
};

void runAll2(Device2* ds[], int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << "    --- 第 " << i + 1 << " 个来源 ---\n";
        int id = -1;
        while (ds[i]->get(id)) {
            std::cout << "    [" << ds[i]->name() << "] 取到第 " << id << " 帧\n";
        }
    }
}

// ============================================================================
//  第 3 节：加上 virtual
// ============================================================================
//  代码只多了一个关键字，循环一个字都没改，输出全对了。

struct Device3 {
    virtual ~Device3() {}                // 顺便：有虚函数的类，析构也要是虚的
    virtual bool get(int& frameId) {     // ← 加上 virtual
        if (next_ >= 5) return false;
        frameId = next_++;
        return true;
    }
    virtual const char* name() const {   // ← 这个也要加，否则名字还是基类的
        return "Device3(基类)";
    }
    int next_ = 0;
};

struct Camera3 : Device3 {
    bool get(int& frameId) override {
        if (next_ >= 3) return false;
        frameId = next_++;
        return true;
    }
    const char* name() const override { return "摄像头"; }
};

struct Video3 : Device3 {
    bool get(int& frameId)  {
        int id = next_ * 2;
        if (id >= 6) return false;
        next_++;
        frameId = id;
        return true;
    }
    const char* name() const override { return "视频文件"; }
};

void runAll3(Device3* ds[], int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << "    --- 第 " << i + 1 << " 个来源 ---\n";
        int id = -1;
        while (ds[i]->get(id)) {
            std::cout << "    [" << ds[i]->name() << "] 取到第 " << id << " 帧\n";
        }
    }
}

// ============================================================================
//  main
// ============================================================================

int main() {
    std::cout << "============================================================\n";
    std::cout << "  第 1 节：不用继承——上层要写两份几乎一样的代码\n";
    std::cout << "============================================================\n";
    Camera1 c1;
    Video1 v1;
    runCamera(c1);
    runVideo(v1);

    std::cout << "\n============================================================\n";
    std::cout << "  第 2 节：用继承但没写 virtual——一个循环了，但结果全错\n";
    std::cout << "============================================================\n";
    std::cout << "  预期：摄像头 3 帧(0,1,2)，视频 3 帧(0,2,4)，名字各自正确\n";
    std::cout << "  实际：\n";
    Camera2 c2;
    Video2 v2;
    Device2* ds2[2] = { &c2, &v2 };
    runAll2(ds2, 2);
    std::cout << "  ↑ 两个都跑成了基类的规则(0..4)，名字都是 Device2(基类)\n";
    std::cout << "  ↑ 注意：这段代码【编译零警告、运行不崩溃】，错误完全静默\n";

    std::cout << "\n============================================================\n";
    std::cout << "  第 3 节：加上 virtual——循环一个字没改，结果全对\n";
    std::cout << "============================================================\n";
    Camera3 c3;
    Video3 v3;
    Device3* ds3[2] = { &c3, &v3 };
    runAll3(ds3, 2);

    std::cout << "\n============================================================\n";
    std::cout << "  第 4 节：virtual 的代价——用一个指针换来的\n";
    std::cout << "============================================================\n";
    std::cout << "  sizeof(Device2) = " << sizeof(Device2) << "  字节（没有虚函数）\n";
    std::cout << "  sizeof(Device3) = " << sizeof(Device3) << "  字节（有虚函数）\n";
    std::cout << "  多出来的部分，主要是一个指针：虚函数表指针(vptr)，8 字节；其余是对齐补水。\n";
    std::cout << "  每个对象里都藏着它，指向一张表，表里记着\"我这一类的 get 在哪\"。\n";
    std::cout << "  所以 d->get() 翻译过来是：查表 → 找到真正的函数 → 调它。\n";
    std::cout << "  这个查表发生在【运行时】，所以叫\"动态绑定\"；\n";
    std::cout << "  没有 virtual 就是编译时定死，叫\"静态绑定\"。\n";

    std::cout << "\n  一句话：virtual 把\"调哪个函数\"的决定权，从写代码的时候\n";
    std::cout << "  推迟到了程序跑起来的时候。\n";

    return 0;
}
