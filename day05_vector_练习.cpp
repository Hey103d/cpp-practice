// day05_vector_练习.cpp
// 今天下午的主线：用 std::vector 替掉你自己维护的数组 + count
//
// 为什么这一步能快：昨天你写的 DetResultList 里，
// 自己管 count、自己判 MAX_OBJ、自己算越界 —— 这三件事 vector 全替你做了。
// 你现在要做的不是"学新东西"，是"把手写的代码换成标准库的"。
//
// 编译：g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day05_vector_练习.cpp -o day05.exe
// 要求：29 个断言全过，并且【零警告】—— 特意留了一个 size_t 陷阱等着你
//
// ===========================================================================
// 概念速览：裸数组 vs vector
// ===========================================================================
//                         你昨天的写法                      vector
//  装东西                 list->results[list->count] = r     v.push_back(r)
//  有几个                 list->count                       v.size()
//  能不能再加             count < MAX_OBJ ?                 永远能，自动扩容
//  清空                   memset / count = 0                v.clear()
//  传参给别人             指针 + 额外传 n                    引用，长度自带
//  遍历                   下标 i 从 0 到 count-1            for (const auto& r : v)
//
// 【三个必须记住的点】
//
// ① v.size() 返回的类型是 size_t（无符号整数！），不是 int。
//    写 for (int i = 0; i < v.size(); ++i) 会触发 -Wsign-compare 警告。
//    两种正确写法：
//      for (std::size_t i = 0; i < v.size(); ++i)
//      for (const DetResult& r : v)              ← 更推荐
//
// ② vector 传参：要改原件就用 std::vector<DetResult>&（引用），
//    只读就用 const std::vector<DetResult>&。
//    【和昨天对比】昨天你写 DetResultList* list，今天写 vector& —— 因为引用更安全，
//    而且 vector 自己知道长度，不需要再传 n。
//
// ③ 数组退化成指针时 sizeof 会变成 8（你踩过）；
//    vector 不会有这个问题，v.size() 永远是真实个数。
//
// ===========================================================================
// 5 题开始
// ===========================================================================

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

struct DetResult {
    int   cls_id;                    // 0=person  2=car
    float prop;
    int   left, top, right, bottom;
    float depth;                     // 米；-1 表示还没测距
};

// 造一个检测结果的小工具（已经给你写好了，测试也在用）
DetResult mkResult(int cls, float prop, int l, int t, int r, int b, float dep) {
    DetResult d{};
    d.cls_id = cls; d.prop = prop;
    d.left = l; d.top = t; d.right = r; d.bottom = b;
    d.depth = dep;
    return d;
}

// ---------------------------------------------------------------------------
// 题 1：构造一帧的检测结果
// 要求：返回一个 vector，里面依次放这 3 个目标（顺序不能错）：
//       ① 行人 0.9  框(100,200,180,340) 深度 3.5
//       ② 汽车 0.8  框(400,210,470,330) 深度 8.0
//       ③ 行人 0.7  框(50,300,120,400)  深度 1.2
// 【提示】std::vector<DetResult> v; 然后 v.push_back(mkResult(...)); 三次，return v;
// ---------------------------------------------------------------------------
std::vector<DetResult> buildFrame() {
    // TODO
    return std::vector<DetResult>();
}

// ---------------------------------------------------------------------------
// 题 2：数某个类别有几个
// 要求：和昨天同一道题，但改用 vector
// 【提示】范围 for：for (const DetResult& r : v) { if (r.cls_id == cls_id) ... }
//         注意这个参数是 const 引用，说明函数只读不写
// ---------------------------------------------------------------------------
int countByClass(const std::vector<DetResult>& v, int cls_id) {
    // TODO
    return 0;
}

// ---------------------------------------------------------------------------
// 题 3：找最近的目标下标（depth 最小，跳过 depth < 0）
// 要求：空 vector 或全部未测距 → 返回 -1
// 【提示】和昨天完全一样，只是遍历方式换成范围 for；
//         但范围 for 拿不到下标 —— 所以这题你还是得用下标遍历，
//         下标类型用 std::size_t（见速览第①点），否则会有警告
// ---------------------------------------------------------------------------
int findNearestIndex(const std::vector<DetResult>& v) {
    // TODO
    return -1;
}

// ---------------------------------------------------------------------------
// 题 4：把框裁进画面（0 <= left < right <= w，0 <= top < bottom <= h）
// 要求：原地修改，所以参数是【非 const 引用】——这是它和昨天指针写法的区别
// 【提示】for (DetResult& r : v)  —— 注意这里没有 const，是引用不是拷贝
// ---------------------------------------------------------------------------
void clampToImage(std::vector<DetResult>& v, int w, int h) {
    // TODO
}

// ---------------------------------------------------------------------------
// 题 5：删掉所有"还没测距"的目标（depth < 0），返回删掉了几个
// 要求：原地从 v 中移除，v 的长度要真的变小
// 【提示】标准做法叫 erase-remove 惯用法，两行：
//           v.erase(std::remove_if(v.begin(), v.end(), 判断函数), v.end());
//         但它对你现在有点绕，所以【允许你自己手写】：
//         用一个新的 vector 装保留的，最后 v = 新vector; 也算对
// ---------------------------------------------------------------------------
int removeUnmeasured(std::vector<DetResult>& v) {
    // TODO
    return 0;
}

// ===========================================================================
// 以下是测试，不要修改
// ===========================================================================
static int g_pass = 0, g_fail = 0;

void check(bool cond, const std::string& name) {
    if (cond) { ++g_pass; std::cout << "  [PASS] " << name << "\n"; }
    else      { ++g_fail; std::cout << "  [FAIL] " << name << "\n"; }
}

int main() {
    std::cout << std::unitbuf;

    // ---- 题 1 ----
    std::cout << "\n[题 1] buildFrame\n";
    {
        std::vector<DetResult> v = buildFrame();
        // 注意下面每条都先 v.size() == 3 &&，靠短路保护 ——
        // 否则空 vector 去访问 v[0] 会越界崩掉，你就看不到后面的测试了。
        // 这和昨天 findNearestIndex 里 best == -1 || 是同一个机制。
        check(v.size() == 3,                                              "一共 3 个目标");
        check(v.size() == 3 && v[0].cls_id == 0,                          "第 1 个是行人");
        check(v.size() == 3 && v[0].left == 100,                          "第 1 个的 left 是 100");
        check(v.size() == 3 && std::fabs(v[1].depth - 8.0f) < 1e-6f,      "第 2 个深度是 8.0");
        check(v.size() == 3 && v[2].right == 120,                         "第 3 个的 right 是 120");
        check(v.size() == 3 && std::fabs(v[2].depth - 1.2f) < 1e-6f,      "第 3 个深度是 1.2");
    }

    // ---- 题 2 ----
    std::cout << "\n[题 2] countByClass\n";
    {
        std::vector<DetResult> v = buildFrame();
        check(countByClass(v, 0) == 2,   "2 个行人");
        check(countByClass(v, 2) == 1,   "1 辆汽车");
        check(countByClass(v, 99) == 0,  "没有的类别返回 0");

        std::vector<DetResult> empty;
        check(countByClass(empty, 0) == 0, "空 vector 返回 0");
        check(v.size() == 3,             "只读函数没改动原 vector");
    }

    // ---- 题 3 ----
    std::cout << "\n[题 3] findNearestIndex\n";
    {
        std::vector<DetResult> v = buildFrame();
        check(findNearestIndex(v) == 2, "最近的是下标 2（1.2 米）");

        std::vector<DetResult> v2;
        v2.push_back(mkResult(0, 0.9f, 100, 200, 180, 340, -1.0f));
        v2.push_back(mkResult(0, 0.8f, 400, 210, 470, 330,  2.0f));
        check(findNearestIndex(v2) == 1, "跳过未测距的，返回下标 1");

        std::vector<DetResult> v3;
        v3.push_back(mkResult(0, 0.9f, 100, 200, 180, 340, -1.0f));
        check(findNearestIndex(v3) == -1, "全都没测距，返回 -1");

        std::vector<DetResult> v4;
        check(findNearestIndex(v4) == -1, "空 vector 返回 -1");
    }

    // ---- 题 4 ----
    std::cout << "\n[题 4] clampToImage\n";
    {
        std::vector<DetResult> v;
        v.push_back(mkResult(0, 0.9f, -20, -30, 100, 120, 3.0f));
        v.push_back(mkResult(0, 0.8f, 600, 400, 700, 500, 2.0f));
        v.push_back(mkResult(0, 0.7f, 100, 100, 200, 200, 1.0f));

        clampToImage(v, 640, 480);
        check(v[0].left == 0,      "负的 left 抬到 0");
        check(v[0].top == 0,       "负的 top 抬到 0");
        check(v[0].right == 100,   "没越界的不动");
        check(v[1].right == 640,   "超出的 right 压到 640");
        check(v[1].bottom == 480,  "超出的 bottom 压到 480");
        check(v[2].left == 100 && v[2].bottom == 200, "范围内完全不动");
        check(std::fabs(v[1].depth - 2.0f) < 1e-6f,  "depth 不许被改动");
        check(v.size() == 3,       "没有增删元素");
    }

    // ---- 题 5 ----
    std::cout << "\n[题 5] removeUnmeasured\n";
    {
        std::vector<DetResult> v;
        v.push_back(mkResult(0, 0.9f, 100, 200, 180, 340,  3.5f));
        v.push_back(mkResult(0, 0.8f, 400, 210, 470, 330, -1.0f));   // 删
        v.push_back(mkResult(2, 0.7f,  50, 300, 120, 400,  1.2f));
        v.push_back(mkResult(0, 0.6f, 300, 220, 350, 300, -1.0f));   // 删

        int removed = removeUnmeasured(v);
        check(removed == 2,        "删掉了 2 个");
        check(v.size() == 2,       "长度真的变小了");
        check(v[0].left == 100,    "第 1 个还在原地");
        check(v[1].cls_id == 2,    "顺序没乱，剩下的是汽车");

        std::vector<DetResult> all;
        all.push_back(mkResult(0, 0.9f, 100, 200, 180, 340, -1.0f));
        check(removeUnmeasured(all) == 1 && all.empty(), "全删光，vector 变空");

        std::vector<DetResult> none;
        check(removeUnmeasured(none) == 0, "空 vector 返回 0");
    }

    std::cout << "\n==========================================\n";
    std::cout << "  通过 " << g_pass << " / 失败 " << g_fail << "\n";
    if (g_fail == 0) std::cout << "  全绿 —— vector 这一关过了\n";
    std::cout << "  别忘了看警告：这一关要求【零警告】\n";
    std::cout << "==========================================\n";
    return 0;
}
