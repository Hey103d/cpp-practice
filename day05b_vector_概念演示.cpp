// day05b_vector_概念演示.cpp
// 目的：把 std::vector 里四个【反直觉】的地方跑给你看。
//       这四处不靠背，靠跑一遍记住现象。
//
// 编译：g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day05b_vector_概念演示.cpp -o day05b.exe
// 预期：【1 个警告，是故意留的教材】
//   warning: variable 'x' set but not used [-Wunused-but-set-variable]
//   出现在第 4 节 for (MiniBox x : v) 那行。
//   这行代码本来就是要演示"改了副本等于没改"，所以"设了没用"正是它的事实。
//   编译器等于替你把"你改的是副本"这句话又说了一遍。
//   —— 除了这 1 个，不该有别的警告。
//
// ===========================================================================
// 为什么需要这个演示？
// ===========================================================================
// 你昨天写 DetResultList 的时候：
//     int count;  DetResult results[64];
// 你自己管 count，所以 count 是 int，你可以写 count >= 0，可以写 count - 1。
// vector 把 count 改名成了 size()，而 size() 返回的是 size_t —— 无符号整数。
// 一个无符号整数，永远 >= 0。
// 你昨天顺手写的那套循环条件，套到 size() 上就会变成死循环。
// 这就是本文件第 1 节要给你看的东西。
//
// ===========================================================================

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>     // remove_if 在这里

// ---------------------------------------------------------------------------
// 第 1 节：size() 返回 size_t，无符号，所以永远 >= 0
// ---------------------------------------------------------------------------
static void demo1_size_t() {
    std::cout << "\n===== 第 1 节：size() 是无符号的 =====\n";

    std::vector<int> empty;
    std::cout << "empty.size()     = " << empty.size() << "\n";
    // 下面这行是重点：0 - 1 在无符号里不会变成 -1，会绕回到最大值。
    std::cout << "empty.size() - 1 = " << empty.size() - 1 << "\n";
    std::cout << "  ↑ 不是 -1，是 2^64-1。空 vector 用它当下标，直接崩。\n";

    std::vector<int> v{10, 20, 30};
    std::cout << "\nv 有 " << v.size() << " 个元素\n";
    std::cout << "v.size() - 1 = " << v.size() - 1 << "  ← 这个才是正常的 2\n";

    // 【错误写法】如果你把下标 i 声明成 size_t，然后想倒着遍历：
    //     for (std::size_t i = v.size() - 1; i >= 0; --i)
    // 条件 i >= 0 对无符号数永远成立，循环不会停，跑到 i 绕回最大值时越界崩。
    //
    // 【正确写法】倒着遍历用反向迭代器，或者把下标转成 int：
    //     for (int i = static_cast<int>(v.size()) - 1; i >= 0; --i)
    //
    // 但绝大多数时候你根本不需要倒着遍历。正着来就行。

    std::cout << "\n【另一个坑】正着遍历别写 int i = 0\n";
    std::cout << "  for (int i = 0; i < v.size(); ++i)  ← 会有 -Wsign-compare 警告\n";
    std::cout << "  正确：for (std::size_t i = 0; i < v.size(); ++i)\n";
    std::cout << "        或者直接用范围 for：for (int x : v)\n";
    std::cout << "  （这条我没法在程序里演给你看，它是编译期警告，不是运行期现象）\n";
}

// ---------------------------------------------------------------------------
// 第 2 节：erase-remove —— remove_if 并不会让 vector 变短
// ---------------------------------------------------------------------------
// 这是整个 vector 里最反直觉的一件事，几乎人人都栽过一次。
// remove_if 做的事情是：把【要保留的】往前挪，然后返回一个迭代器，
// 指向"最后一个有效元素的下一个位置"。容器长度【一个都没少】。

static bool isNeg(int x) {
    return x < 0;
}

static void printVec(const std::string& tag, const std::vector<int>& v) {
    std::cout << tag << " size=" << v.size() << "  内容: ";
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

static void demo2_erase_remove() {
    std::cout << "\n===== 第 2 节：remove_if 之后 size 居然没变 =====\n";

    std::vector<int> v{1, -2, 3, -4, 5};
    printVec("原始          ", v);

    // remove_if 只是把留下来的元素往前挤，返回"新逻辑结尾"
    std::vector<int>::iterator newEnd = std::remove_if(v.begin(), v.end(), isNeg);

    printVec("remove_if 之后", v);
    std::cout << "  ↑ size 还是 5！后面的 -4 5 是【残留垃圾】，不该再读\n";

    std::cout << "  newEnd - v.begin() = " << (newEnd - v.begin())
              << "   ← 这才是真正该保留的个数\n";

    // erase 才是真的把尾巴砍掉
    v.erase(newEnd, v.end());
    printVec("erase 之后    ", v);
    std::cout << "  ↑ 现在 size 才真的变成 3\n";

    std::cout << "\n【一口气写法】实际代码里都是两行合成一行：\n";
    std::cout << "  v.erase(std::remove_if(v.begin(), v.end(), isNeg), v.end());\n";
    std::cout << "  先 remove_if 挤，再 erase 砍。两个容器函数，缺一个都不行。\n";

    std::cout << "\n【你写练习时可以用更土的办法】\n";
    std::cout << "  新建一个 vector 装要保留的，遍历完 v = 新vector;\n";
    std::cout << "  vector 的 = 是整体替换，长度跟着变。这个办法不优雅但绝不会错。\n";
}

// ---------------------------------------------------------------------------
// 第 3 节：push_back 超过容量会"搬家"，之前拿到的指针全部作废
// ---------------------------------------------------------------------------
struct MiniBox {
    int depth;
};

static void demo3_reallocate() {
    std::cout << "\n===== 第 3 节：扩容 = 搬家 = 旧指针全部作废 =====\n";

    std::vector<MiniBox> v;
    std::cout << "刚建好      capacity=" << v.capacity()
              << "  数据地址=" << static_cast<const void*>(v.data()) << "\n";

    // 先只预留 2 个位置，把"第 3 个装不下"这件事锁死
    v.reserve(2);
    std::cout << "reserve(2)  capacity=" << v.capacity()
              << "  数据地址=" << static_cast<const void*>(v.data()) << "\n";

    v.push_back(MiniBox{3});
    v.push_back(MiniBox{4});
    std::cout << "装 2 个     capacity=" << v.capacity()
              << "  数据地址=" << static_cast<const void*>(v.data()) << "\n";

    // 记下这一刻第 0 个元素的地址
    const MiniBox* oldPtr = &v[0];

    // 关键一步：在老内存后面紧跟着占掉一大块，
    // 这样 vector 想"原地长大"也没地方，必须搬家。
    // （不这么做的话，Windows 堆偶尔会原地扩，你就看不到地址变化了 —— 那会造成误解）
    std::vector<char> blocker(4096, 'x');
    std::cout << "占住后面 4096 字节，地址=" << static_cast<const void*>(blocker.data()) << "\n";

    v.push_back(MiniBox{5});        // 第 3 个：capacity 2 → 4，必然重新分配

    std::cout << "\n装第 3 个   capacity=" << v.capacity() << "  ← 容量翻倍了，说明真的重新分配了\n";
    std::cout << "老地址 &v[0] = " << static_cast<const void*>(oldPtr)
              << "  ← 你之前记下的\n";
    std::cout << "新地址 &v[0] = " << static_cast<const void*>(&v[0])
              << "  ← 现在的\n";
    std::cout << "  ↑ 两个不一样。vector 申请了一块更大的内存，把老数据搬过去，\n";
    std::cout << "    然后【释放老内存】。oldPtr 现在指向已经还给系统的内存 —— 悬空指针。\n";

    // 顺便：释放 blocker，免得它一直占着
    blocker.clear();
    blocker.shrink_to_fit();

    std::cout << "\n【怎么避免】如果你知道最多会有 64 个目标，一开始就 reserve(64)：\n";
    std::vector<MiniBox> w;
    w.reserve(64);
    std::cout << "  w.reserve(64) 后 capacity=" << w.capacity() << "\n";
    const void* addr0 = w.data();
    for (int i = 0; i < 64; ++i) w.push_back(MiniBox{i});
    std::cout << "  装满 64 个后 capacity=" << w.capacity()
              << "  地址还是=" << static_cast<const void*>(w.data()) << "\n";
    std::cout << "  ↑ 一次都没搬。地址从头到尾没变（和上面记的 "
              << static_cast<const void*>(addr0) << " 一样）。\n";
    std::cout << "  你毕设里 MAX_OBJ 是 64 —— 这就是 reserve(64) 的用处。\n";

    std::cout << "\n【工程含义】别长期保存 &v[i] 或者指向元素的指针，\n";
    std::cout << "  只要后面还会 push_back 且没 reserve 够，它随时可能失效。\n";
    std::cout << "  要跨帧记住某个目标，存 int idx，别存 DetResult*。\n";
}

// ---------------------------------------------------------------------------
// 第 4 节：范围 for 加不加 & 的区别
// ---------------------------------------------------------------------------
static void demo4_range_for() {
    std::cout << "\n===== 第 4 节：for (Box x : v) 和 for (Box& x : v) =====\n";

    std::vector<MiniBox> v(3);      // 3 个元素，depth 都是 0
    v[0].depth = 11; v[1].depth = 22; v[2].depth = 33;
    std::cout << "初始      : " << v[0].depth << " " << v[1].depth << " " << v[2].depth << "\n";

    // 注意：下面这一行会得到一个 -Wunused-but-set-variable 警告。
    // 因为 x 是副本，你给它赋了 99 之后没有任何人再读它 —— 编译器看得一清二楚。
    // 这个警告本身就是"你改的是副本"的机器版证明。
    for (MiniBox x : v) {           // 没有 & —— x 是【副本】
        x.depth = 99;
    }
    std::cout << "for(Box x :v) 之后: " << v[0].depth << " " << v[1].depth << " " << v[2].depth
              << "   ← 一个都没变，改的是副本\n";

    for (MiniBox& x : v) {          // 有 & —— x 是【原件】
        x.depth = 99;
    }
    std::cout << "for(Box& x:v) 之后: " << v[0].depth << " " << v[1].depth << " " << v[2].depth
              << "   ← 全变了\n";

    std::cout << "\n【怎么选】三选一，别想太久：\n";
    std::cout << "  只读，不想改  → for (const MiniBox& x : v)   最常用\n";
    std::cout << "  要改原件      → for (MiniBox& x : v)\n";
    std::cout << "  要下标        → 用不了范围 for，退回 for (std::size_t i = 0; ...)\n";

    std::cout << "\n  const 引用还有一个好处：它挡住你手滑改数据。\n";
    std::cout << "  在 const 引用上写 x.depth = 1 会【编译报错】，而不是默默改坏。\n";
}

// ===========================================================================
int main() {
    std::cout << std::unitbuf;      // 崩了也要把前面的输出吐出来（你学过这个）

    std::cout << "==========================================\n";
    std::cout << "  vector 的四个反直觉之处\n";
    std::cout << "==========================================\n";

    demo1_size_t();
    demo2_erase_remove();
    demo3_reallocate();
    demo4_range_for();

    std::cout << "\n==========================================\n";
    std::cout << "  跑完了。带着这四节的印象去看 day05_vector_练习.cpp\n";
    std::cout << "==========================================\n";
    return 0;
}
