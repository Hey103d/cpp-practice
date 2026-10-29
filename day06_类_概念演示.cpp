// day06_类_概念演示.cpp
// 主题：类与对象（上）—— 构造 / 析构 / 封装 / 成员函数 / this / const 成员函数
//
// 编译：g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day06_类_概念演示.cpp -o day06.exe
// 预期：【1 个警告，是故意留的教材】
//   warning: declaration of 'depth' shadows a member of 'Probe' [-Wshadow]
//   出现在第 4 节 Probe::setDepth 那里。
//   那一行就是要演示"参数名和成员名撞了"，所以"遮蔽"正是它的事实。
//   编译器等于替你把"必须写 this-> 的原因"又说了一遍。
//   —— 除了这 1 个，不该有别的警告。
//
// ===========================================================================
// 一句话抓住这一课
// ===========================================================================
// 你前面几天写的是【散装的】函数：数据一处、操作一处，靠参数把两边连起来。
// 类做的事只有一件：**把数据和操作数据的方法绑在一起，并且规定谁能碰数据。**
//
// 昨天你写 5 个函数，第一件事就是 `const std::vector<DetResult>& v`，
// 每次都要把 v 传进去。写进类之后，v 变成成员变量，函数不用再收这个参数了。
// 这不是省事，是**让它不可能传错**。
// ===========================================================================

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

// ---------------------------------------------------------------------------
// 第 1 节：class 和 struct 只差一件事 —— 默认访问权限
// ---------------------------------------------------------------------------
// 这可能是最容易误解的一点：class 和 struct 在 C++ 里【几乎一样】，
// 唯一的区别是默认访问权限：
//     struct  → 默认 public
//     class   → 默认 private
// 习惯上：纯数据集合用 struct，有行为、有不变式的用 class。

struct SPoint {
    int x;          // 默认 public，外面能直接改
    int y;
};

class CPoint {
    int x;          // 默认 private，外面碰不到
    int y;
public:
    void set(int a, int b) { x = a; y = b; }
    int sum() const { return x + y; }
};

static void demo1_struct_vs_class() {
    std::cout << "\n===== 第 1 节：class 和 struct 的差别 =====\n";

    SPoint s;
    s.x = 3; s.y = 4;
    std::cout << "struct  SPoint: 外面直接写 s.x = 3，成功，s.x = " << s.x << "\n";

    CPoint c;
    c.set(3, 4);
    std::cout << "class   CPoint: 只能通过 c.set(3, 4) 改，c.sum() = " << c.sum() << "\n";
    std::cout << "  ↑ 如果你写 c.x = 3; 编译器会报：\n";
    std::cout << "    error: 'int CPoint::x' is private within this context\n";
    std::cout << "  【关键】这不是 class 更高级，是它默认把门关上了。\n";
    std::cout << "  你在 struct 里写 public: / private: 效果完全一样。\n";
}

// ---------------------------------------------------------------------------
// 第 2 节：构造函数 —— 对象生下来就要有初始状态
// ---------------------------------------------------------------------------
class Counter {
public:
    Counter() : n_(0) {                       // 默认构造：不带参数也能建对象
        std::cout << "  Counter() 被调用\n";
    }
    Counter(int n) : n_(n) {                  // 带参构造
        std::cout << "  Counter(int) 被调用，n = " << n << "\n";
    }
    int value() const { return n_; }
private:
    int n_;
};

static void demo2_constructor() {
    std::cout << "\n===== 第 2 节：构造函数 =====\n";

    std::cout << "Counter a;        → ";  Counter a;
    std::cout << "Counter b(5);     → ";  Counter b(5);
    std::cout << "Counter c{7};     → ";  Counter c{7};

    std::cout << "a.value()=" << a.value()
              << "  b.value()=" << b.value()
              << "  c.value()=" << c.value() << "\n";

    std::cout << "\n【为什么冒号后面那个写法叫初始化列表】\n";
    std::cout << "  Counter(int n) : n_(n) { }   ← 冒号到花括号之间\n";
    std::cout << "  它和写在函数体里 n_ = n; 有区别，而且有时候【只能用冒号写法】：\n";
    std::cout << "    · const 成员变量   —— 只有一次赋值机会\n";
    std::cout << "    · 引用成员变量     —— 引用必须初始化\n";
    std::cout << "  写进函数体里就是\"先默认构造一次，再赋值一次\"，浪费且有时候直接编译不过。\n";

    std::cout << "\n【构造函数不需要写返回类型】这不是笔误，是规定。\n";
    std::cout << "  名字必须和类名完全相同，也没有 void。\n";
}

// ---------------------------------------------------------------------------
// 第 3 节：析构函数 —— 对象怎么死，以及死的顺序
// ---------------------------------------------------------------------------
// 语法：~类名()，没有参数、没有返回值、不能重载（只有一个）。
// 什么时候自动调用：局部对象离开作用域、delete 一个指针、容器被销毁。
// 顺序：**后构造的先析构**（像栈，后进先出）。

class Tracer {
public:
    Tracer(const std::string& name) : name_(name) {
        std::cout << "  构造 " << name_ << "\n";
    }
    ~Tracer() {
        std::cout << "  析构 " << name_ << "\n";
    }
private:
    std::string name_;
};

static void demo3_destructor() {
    std::cout << "\n===== 第 3 节：析构函数与生命周期 =====\n";

    std::cout << "进入一个 { } 块：\n";
    {
        Tracer a("A");
        Tracer b("B");
        Tracer c("C");
        std::cout << "  （块内代码执行中）\n";
    }
    std::cout << "离开 { } 块之后 ↑ 看到逆序析构：C、B、A\n";
    std::cout << "  【为什么逆序】因为构造是往里加，析构是从里往外拆。\n";
    std::cout << "  如果 C 依赖 B，C 必须先死，否则 B 没了 C 还活着 → 崩。\n";

    std::cout << "\n放到 vector 里也一样：\n";
    {
        std::vector<Tracer> v;
        std::cout << "  push 三个：\n";
        v.emplace_back("V0");
        v.emplace_back("V1");
        std::cout << "  现在 v.clear()：\n";
        v.clear();
        std::cout << "  clear 之后 vector 长度 = " << v.size() << "\n";
    }
    std::cout << "  ↑ 容器销毁时，里面的每个元素都会被析构。\n";
    std::cout << "  所以你用 std::vector 装数据，**根本不用手写析构函数** ——\n";
    std::cout << "  vector 自己会把内存还掉。这正是昨天 vector 替你省掉的那件事。\n";

    std::cout << "\n【重要】析构函数不是必须写的。\n";
    std::cout << "  只有你自己 new 了内存、或者打开了文件/设备句柄，才需要写析构来收尾。\n";
    std::cout << "  你的 DetFrame 里只有 vector，所以【不用写析构】。\n";
}

// ---------------------------------------------------------------------------
// 第 4 节：this —— 指向"我自己"的指针
// ---------------------------------------------------------------------------
class Probe {
public:
    // 参数名和成员名一样时，不加 this-> 就分不清
    // 注意：这一行会触发 -Wshadow 警告，因为 depth 遮蔽了同名成员。
    // 警告本身就是这一节的结论 —— 参数撞名是有代价的。
    void setDepth(float depth) {
        this->depth = depth;          // this->depth 是成员，depth 是参数
    }
    float getDepth() const { return depth; }

    // 实测：this 就是对象自己的地址
    const void* myAddress() const { return this; }

private:
    float depth = -1.0f;              // 成员默认值，比构造函数更省事
};

static void demo4_this() {
    std::cout << "\n===== 第 4 节：this 指针 =====\n";

    Probe p;
    p.setDepth(3.5f);

    std::cout << "对象地址 &p        = " << static_cast<const void*>(&p) << "\n";
    std::cout << "成员函数里的 this  = " << p.myAddress() << "\n";
    std::cout << "  ↑ 两者相同。this 就是\"指向当前这个对象的指针\"。\n";

    std::cout << "\n【this 什么时候必须用】只有一个场景：\n";
    std::cout << "  参数名和成员名撞了。setDepth(float depth) 里两个 depth，\n";
    std::cout << "  不加 this-> 就是参数给自己赋值，什么也没发生（而且编译器不一定警告）。\n";
    std::cout << "  【推荐做法】成员变量加下划线后缀 depth_ ，从根上避开重名，\n";
    std::cout << "  这样连 this-> 都不用写，还不会撞。\n";

    std::cout << "\n【顺带】struct 里的 float depth = -1.0f; 这种\"成员默认值\"很实用，\n";
    std::cout << "  它让每个新建对象的 depth 一开始就是 -1，正好是\"还没测距\"。\n";
}

// ---------------------------------------------------------------------------
// 第 5 节：const 成员函数 —— 昨天那道题的答案，编译器帮你守
// ---------------------------------------------------------------------------
class Meter {
public:
    void setValue(float v) { value_ = v; }      // 会改，不能 const
    float value() const { return value_; }      // 只读，标 const
private:
    float value_ = 0.0f;
};

static void demo5_const_member() {
    std::cout << "\n===== 第 5 节：const 成员函数 =====\n";

    Meter m;
    m.setValue(2.5f);
    std::cout << "普通对象可以调 setValue 和 value：m.value() = " << m.value() << "\n";

    const Meter cm = m;                          // const 对象
    std::cout << "const 对象能调 value()：cm.value() = " << cm.value() << "\n";
    std::cout << "const 对象调 setValue() 会怎样？\n";
    std::cout << "  error: passing 'const Meter' as 'this' argument discards qualifiers\n";
    std::cout << "  ↑ 意思是：你拿一个不许改的对象，去调一个可能改它的函数，我不给。\n";

    std::cout << "\n【本质】const 成员函数 = 保证 this 是 const 的\n";
    std::cout << "  所以在 const 成员函数里改成员变量，编译器直接报错。\n";
    std::cout << "  这不是为了好看，是为了**让错误在编译期暴露**，而不是运行时数据被改坏。\n";

    std::cout << "\n【昨天的题】countByClass / findNearestIndex 只读 → 加 const\n";
    std::cout << "            clampToImage / removeUnmeasured 会改 → 不加\n";
    std::cout << "  标错方向的后果：加少了只是没享受到保护；\n";
    std::cout << "  **加多了直接编译不过**（因为函数体里真的改了成员）。\n";
}

// ---------------------------------------------------------------------------
// 第 6 节：封装 —— 把门关上，只留一个带检查的窗口
// ---------------------------------------------------------------------------
// 封装的真正价值不是"藏起来"，而是**只留一条通道，在通道口做校验**。
// 你前几天反复纠结的"数据脏了怎么办"，答案就在这：让脏数据根本进不来。

class SafeDepth {
public:
    // 唯一的入口：负值一律归成 -1（未测距）
    void set(float d) {
        if (d < 0.0f) {
            depth_ = -1.0f;
            ++rejected_count_;
        } else {
            depth_ = d; 
        }
    }
    float get() const { return depth_; }
    int rejected() const { return rejected_count_; }

private:
    float depth_         = -1.0f;
    int   rejected_count_ = 0;
};

static void demo6_encapsulation() {
    std::cout << "\n===== 第 6 节：封装 = 唯一入口 + 入口校验 =====\n";

    SafeDepth d;
    d.set(3.5f);
    std::cout << "set(3.5)  → get() = " << d.get() << "\n";
    d.set(-8.0f);
    std::cout << "set(-8.0) → get() = " << d.get()
              << "  （被归成 -1，不是 -8）\n";
    std::cout << "被拦下的次数 = " << d.rejected() << "\n";

    std::cout << "\n【对比】如果 depth_ 是 public：\n";
    std::cout << "  外面随手写 obj.depth_ = -8.0f; 你的 findNearestIndex 就会以为\n";
    std::cout << "  \"距离 -8 米\"，一路算出莫名其妙的结果，而且**没有任何一处报错**。\n";
    std::cout << "\n【这就是你前几天那个问题的终极答案】\n";
    std::cout << "  \"数据脏了怎么办\" → 让它进不来。把成员关成 private，\n";
    std::cout << "  只在 setter 里做校验，外面就没有机会塞脏数据。\n";
    std::cout << "  比\"事后到处 if 判断\"干净一个量级。\n";
}

// ===========================================================================
int main() {
    std::cout << std::unitbuf;

    std::cout << "==============================================\n";
    std::cout << "  类与对象（上）\n";
    std::cout << "==============================================\n";

    demo1_struct_vs_class();
    demo2_constructor();
    demo3_destructor();
    demo4_this();
    demo5_const_member();
    demo6_encapsulation();

    std::cout << "\n==============================================\n";
    std::cout << "  六句话带走：\n";
    std::cout << "  1. class 和 struct 只差默认访问权限\n";
    std::cout << "  2. 构造函数没返回类型，初始化列表能办到函数体办不到的事\n";
    std::cout << "  3. 析构后进先出；用 vector 就不用手写析构\n";
    std::cout << "  4. this = 对象自己的地址，只在参数撞名时必须写\n";
    std::cout << "  5. const 成员函数 = 保证不改对象，标错了编译不过\n";
    std::cout << "  6. 封装的本质 = 唯一入口 + 入口校验，脏数据进不来\n";
    std::cout << "==============================================\n";
    return 0;
}
