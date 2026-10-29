// day05c_data与强制转换.cpp
// 回答两个问题：
//   ① v.data() 是什么？
//   ② 为什么要 static_cast，C 风格 (const void*) 不行吗？
//
// 编译：g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day05c_data与强制转换.cpp -o day05c.exe
// 预期：零警告
//
// ===========================================================================
// 先给结论（这三句可以背下来）
// ===========================================================================
// ① v.data() = 指向 vector 内部数组第一个元素的指针，等价于 &v[0]。
//    它的真正用途：把 vector 交给只认指针的老接口（OpenCV、RKNN 全是这种）。
//
// ② cout 打地址必须转成 void*，但【只对 char 家族是必需的】。
//    对 MiniBox* 不转也能打 —— 我演示里那个 static_cast 是多余的。
//
// ③ static_cast 和 C 风格 (类型) 的区别不是"哪个能过编译"，
//    而是"谁在你做错事的时候拦你"。C 风格什么都答应，static_cast 会拒绝。
//
// ===========================================================================

#include <iostream>
#include <vector>
#include <string>

struct MiniBox {
    int depth;
};

// ---------------------------------------------------------------------------
// 第 1 节：v.data() 到底是什么
// ---------------------------------------------------------------------------
static void demo1_what_is_data() {
    std::cout << "\n===== 第 1 节：v.data() 是什么 =====\n";

    std::vector<MiniBox> v;
    v.reserve(3);
    v.push_back(MiniBox{11});
    v.push_back(MiniBox{22});
    v.push_back(MiniBox{33});

    // data() 返回的就是第 0 个元素的地址
    std::cout << "&v[0]           = " << static_cast<const void*>(&v[0]) << "\n";
    std::cout << "v.data()        = " << static_cast<const void*>(v.data()) << "\n";
    std::cout << "两者相等吗      = " << (v.data() == &v[0] ? "相等" : "不等") << "\n";

    // 它是 MiniBox* 类型，可以直接当下标用
    std::cout << "v.data()[0].depth = " << v.data()[0].depth << "\n";
    std::cout << "v.data()[2].depth = " << v.data()[2].depth << "\n";
    std::cout << "  ↑ v.data()[i] 和 v[i] 完全等价，只是写法难看\n";

    // 空 vector 时没有"第 0 个元素"，data() 可能返回 nullptr
    std::vector<MiniBox> empty;
    std::cout << "\n空 vector 的 data() = " << static_cast<const void*>(empty.data()) << "\n";
    std::cout << "  ↑ 空 vector 千万不要用 data() 去解引用，那和空指针一样\n";

    // 【这才是 data() 存在的真正理由】
    std::cout << "\n【为什么需要它】很多老接口只认【指针 + 长度】，不认 vector：\n";
    std::cout << "  OpenCV 的 Mat 构造函数、RKNN 的 rknn_input 都是这种。\n";
    std::cout << "  你毕设里把图像数据交给 RKNN 时，就得写：\n";
    std::cout << "      input.buf = img.data();\n";
    std::cout << "      input.size = img.size() * sizeof(unsigned char);\n";
    std::cout << "  vector 负责管内存，data() 负责把裸指针借出去。就这一个用途。\n";
}

// ---------------------------------------------------------------------------
// 第 2 节：cout 打地址，什么时候必须转 void*
// ---------------------------------------------------------------------------
static void demo2_why_void_ptr() {
    std::cout << "\n===== 第 2 节：什么时候必须转 void* =====\n";

    std::vector<MiniBox> v;
    v.push_back(MiniBox{7});

    // 情况 A：MiniBox* —— 不转也能打地址
    std::cout << "MiniBox* 不转   : " << v.data() << "\n";
    std::cout << "MiniBox* 转了   : " << static_cast<const void*>(v.data()) << "\n";
    std::cout << "  ↑ 两个输出一模一样。所以演示里那句 static_cast 是【多余的】，我认。\n";

    // 情况 B：char 家族 —— 不转就【打不出地址】，会打成字符串内容
    const char* s = "hello";
    std::cout << "\nconst char* 不转: " << s << "\n";
    std::cout << "const char* 转了: " << static_cast<const void*>(s) << "\n";
    std::cout << "  ↑ 不转打印的是【hello】，转成 void* 才打印地址\n";

    char buf[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', '\0'};
    std::cout << "char[8]   不转  : " << buf << "\n";
    std::cout << "char[8]   转了  : " << static_cast<const void*>(buf) << "\n";
    std::cout << "  ↑ char 数组同理。因为 ostream 给 char* 专门准备了一个版本，\n";
    std::cout << "    它认为【你给 char 指针就是想打字符串】。\n";

    // 【这条为什么会咬到你】
    std::cout << "\n【为什么会咬到你】unsigned char 也属于 char 家族，同样被当成字符串。\n";
    std::cout << "  RKNN 的图像 buffer 就是 unsigned char* —— 你想打印它的地址，\n";
    std::cout << "  不转 void* 就会打出一堆乱码。这是你调试模型输入时一定会遇到的。\n";

    std::cout << "\n【规则】打印任何指针的地址，一律写成 static_cast<const void*>(p)。\n";
    std::cout << "  对 MiniBox* 是多余的，但对 char* 是必需的。\n";
    std::cout << "  统一这么写就不用每次先想【这个类型是哪一类】。\n";
}

// ---------------------------------------------------------------------------
// 第 3 节：static_cast 和 C 风格 (类型) 的真正区别
// ---------------------------------------------------------------------------
static void demo3_static_vs_c() {
    std::cout << "\n===== 第 3 节：static_cast vs C 风格 =====\n";

    // ---- 场景一：把 const 去掉 ----
    std::cout << "\n--- 场景一：有个 const 指针，你想转成非 const ---\n";
    MiniBox b{7};
    const MiniBox* cp = &b;

    MiniBox* p1 = (MiniBox*)cp;      // C 风格：悄悄过
    p1->depth = 99;                  // 然后就能改了
    std::cout << "C 风格 (MiniBox*)cp 之后，b.depth = " << b.depth << "  ← 编译通过，改成功了\n";

    std::cout << "static_cast<MiniBox*>(cp) 呢？编译【报错】：\n";
    std::cout << "  error: static_cast from type 'const MiniBox*' to type 'MiniBox*'\n";
    std::cout << "         casts away qualifiers\n";
    std::cout << "  ↑ casts away qualifiers 意思是【把 const 这个限定符扔掉了】\n";

    // ---- 场景二：两个毫无关系的类型 ----
    std::cout << "\n--- 场景二：两个毫无关系的结构体 ---\n";
    struct A { int x; };
    struct B { double y; };

    B obj{2.5};
    A* pa = (A*)&obj;                // C 风格：照样过
    std::cout << "C 风格 (A*)&obj 之后，pa->x = " << pa->x
              << "   ← 期望 2.5，实际是垃圾\n";
    std::cout << "  【注意】它不是乱码，是个看着很正常的 0。\n";
    std::cout << "  2.5 的 double 字节低 4 位正好是 0，读成 int 就是 0。\n";
    std::cout << "  所以你根本看不出错了 —— 这种【看着对】的错最难查。\n";

    std::cout << "static_cast<A*>(&obj) 呢？编译【报错】：\n";
    std::cout << "  error: invalid static_cast from type 'B*' to type 'A*'\n";
    std::cout << "  ↑ 编译器直接说：这俩没关系，我不给你转\n";

    // ---- 结论 ----
    std::cout << "\n--- 结论 ---\n";
    std::cout << "  (类型)expr        = 【什么都答应你】，编译器不检查语义\n";
    std::cout << "  static_cast<T>(expr) = 【只在语义讲得通时才答应】\n";
    std::cout << "\n  所以在 C++ 里，(类型) 这种写法基本可以当成禁用。\n";
    std::cout << "  它没有变快，也没有变强，只是把检查关掉了。\n";
}

// ---------------------------------------------------------------------------
// 第 4 节：static_cast 拦不住的两件事（别以为它是万能的）
// ---------------------------------------------------------------------------
static void demo4_static_cast_cant() {
    std::cout << "\n===== 第 4 节：static_cast 拦不住什么 =====\n";

    std::cout << "static_cast 不是安全网，它只查【类型关系讲不讲得通】。\n";
    std::cout << "\n【拦不住的 1】数组越界 —— 它不管你转的东西有没有指向有效内存\n";
    std::vector<MiniBox> v;
    v.push_back(MiniBox{1});
    MiniBox* p = static_cast<MiniBox*>(v.data());
    std::cout << "  把 v.data() 转成 MiniBox* 完全合法，p[0].depth = " << p[0].depth << "\n";
    std::cout << "  但 vector 里只有 1 个元素，你去读 p[5] 照样是越界。\n";
    std::cout << "  static_cast 一句话都不会说 —— 它只管类型，不管内存。\n";

    std::cout << "\n【拦不住的 2】void* 转回来 —— 这个恰恰是合法的，你需要它\n";
    std::cout << "  static_cast<T*>(voidPtr) 是允许的，因为 void* 就是【不知道是什么类型】，\n";
    std::cout << "  转回具体类型属于正常操作。RKNN 里到处是这种：\n";
    std::cout << "      unsigned char* data = static_cast<unsigned char*>(input.buf);\n";

    std::cout << "\n【那真正危险的转换用什么】\n";
    std::cout << "  把 double* 当 int* 用、把 A* 当 B* 用 —— 这些叫 reinterpret_cast，\n";
    std::cout << "  它才是【我明确知道自己要重新解释这段内存】。\n";
    std::cout << "  规则很简单：写 static_cast 编译不过，而你又确信必须这么干，\n";
    std::cout << "  才换成 reinterpret_cast。C 风格 (类型) 两者都替你干了、还不告诉你干了哪个。\n";
}

// ===========================================================================
int main() {
    std::cout << std::unitbuf;

    std::cout << "==============================================\n";
    std::cout << "  v.data() 与强制转换\n";
    std::cout << "==============================================\n";

    demo1_what_is_data();
    demo2_why_void_ptr();
    demo3_static_vs_c();
    demo4_static_cast_cant();

    std::cout << "\n==============================================\n";
    std::cout << "  记住三句：\n";
    std::cout << "  1. data() = &v[0]，用来喂只认指针的老接口\n";
    std::cout << "  2. 打地址一律 static_cast<const void*>(p)，char 家族不转会打成字符串\n";
    std::cout << "  3. (类型) 什么都答应，static_cast 会拒绝，所以别用 (类型)\n";
    std::cout << "==============================================\n";
    return 0;
}
