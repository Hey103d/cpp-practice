// day06_类_盲写练习.cpp
// 主题：把 day05 的 5 个散函数，全部搬进一个类里
//
// 编译：g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day06_类_盲写练习.cpp -o day06p.exe
// 要求：34 个断言全过，并且【零警告】
//
// ===========================================================================
// 这一课到底在练什么
// ===========================================================================
// day05 你写的是这样：
//     std::vector<DetResult> buildFrame();
//     int countByClass(const std::vector<DetResult>& v, int cls_id);
//     int findNearestIndex(const std::vector<DetResult>& v);
//     void clampToImage(std::vector<DetResult>& v, int w, int h);
//     int removeUnmeasured(std::vector<DetResult>& v);
// 每个函数都要把 v 传进去。传参就有可能传错：传成空 vector、传错对象、忘了判空。
//
// 写进类之后：
//     DetFrame frame;
//     frame.addResult(r);
//     frame.countByClass(0);
//     frame.findNearestIndex();
//     frame.clampToImage(640, 480);
//     frame.removeUnmeasured();
// **不用再传 v 了。** 因为它就是对象自己的一部分，不存在"传错"这回事。
//
// ===========================================================================
// 四个必须记住的点
// ===========================================================================
// ① 成员变量名后面加下划线（results_），这样和参数名天然不撞，不用写 this->。
// ② 只读的函数必须加 const。没加的话，const 对象就调不了它，编译报错。
//    本题里 countByClass / findNearestIndex / size / empty / results 都是只读的。
// ③ 成员变量放 private，别人只能通过 public 函数碰它。
// ④ 类里用 vector 装数据，**不需要写析构函数**，vector 自己会释放。

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

struct DetResult {
    int   cls_id;
    float prop;
    int   left, top, right, bottom;
    float depth;                     // 米；-1 表示还没测距
};

// 造一条检测结果（已经给你写好了）
DetResult mkResult(int cls, float prop, int l, int t, int r, int b, float dep) {
    DetResult d{};
    d.cls_id = cls; d.prop = prop;
    d.left = l; d.top = t; d.right = r; d.bottom = b;
    d.depth = dep;
    return d;
}

// ===========================================================================
// 一个类 = 一帧的检测结果
// ===========================================================================
// 【注意 const 的位置】函数名后面、花括号前面那个 const，意思是
//   "这个函数不会改动对象自己"。
// size() const 表示：调用它不会改变 results_。
// 这也是为什么 const DetFrame 对象能调用 size()，却调不了 addResult()。

class DetFrame {
public:
    // -----------------------------------------------------------------------
    // 题 1：两个构造函数
    // 要求：
    //   ① DetFrame()        默认构造，建出一个空帧（results_ 是空 vector，
    //                       不需要你写任何代码，但你要让它能编译）
    //   ② DetFrame(const std::vector<DetResult>& v)
    //                       用一组已有的结果构造，把 v 里的内容复制进 results_
    // 【提示】构造函数没有返回类型；初始化列表写法是
    //           DetFrame(const std::vector<DetResult>& v) : results_(v) { }
    // -----------------------------------------------------------------------
    DetFrame() = default;

    explicit DetFrame(const std::vector<DetResult>& v) {
        // TODO
        results_=v;
    }

    // -----------------------------------------------------------------------
    // 题 2：往里加一条结果，以及报出当前有几条
    // 要求：addResult 把 r 追加到末尾；size 返回条数；empty 返回是否为空
    // 【提示】vector 的 push_back / size / empty 直接用即可
    // -----------------------------------------------------------------------
    void addResult(const DetResult& r) {
        // TODO
        results_.push_back(r);
    }

    std::size_t size() const {
        // TODO
        return results_.size();
    }

    bool empty() const {
        // TODO
        return results_.empty();
    } 

    // -----------------------------------------------------------------------
    // 题 3：数某个类别有几个
    // 要求：和 day05 一样，只是不用再传 vector 了，直接遍历 results_
    // 【提示】for (const DetResult& r : results_) { ... }
    // -----------------------------------------------------------------------
    int countByClass(int cls_id) const {
        // TODO
        int cn=0;
        for(const DetResult &r:results_){
            if(r.cls_id==cls_id){
                cn++;
            }
        }
        return cn;
    }

    // -----------------------------------------------------------------------
    // 题 4：找最近目标的下标（depth 最小，跳过 depth < 0）
    // 要求：空帧或全部未测距 → 返回 -1
    // 【提示】day05 写过一遍了。注意 size() 是 size_t，下标别用 int。
    // -----------------------------------------------------------------------
    int findNearestIndex() const {
        // TODO
        int best=-1;
        for(size_t i=0;i<results_.size();i++){
            if(results_[i].depth<0)continue;
            if(best==-1||results_[i].depth<results_[best].depth){
                best=i;
            }
        }
        return best;
    }

    // -----------------------------------------------------------------------
    // 题 5：把所有框裁进画面
    // 要求：两个方向都要裁（不只是 left<0 和 right>w）
    // 【提示】std::clamp(x, lo, hi) 一次管两头。左上右下四个都要 clamp。
    // -----------------------------------------------------------------------
    void clampToImage(int w, int h) {
        // TODO
        for(size_t i=0;i<results_.size();i++){
            results_[i].left=std::clamp(results_[i].left,0,w);
            results_[i].right=std::clamp(results_[i].right,0,w);
            results_[i].top=std::clamp(results_[i].top,0,h);
            results_[i].bottom=std::clamp(results_[i].bottom,0,h);
        }
    }

    // -----------------------------------------------------------------------
    // 题 6：删掉所有"还没测距"的目标，返回删掉了几个
    // 要求：results_ 的长度要真的变小
    // 【提示】erase-remove；个数用 results_.end() - newEnd 算
    // -----------------------------------------------------------------------
   static bool isunmeasured(const DetResult& r){
        return r.depth<0;
   }
   
   
    int removeUnmeasured() {
        // TODO
        std::vector<DetResult>::iterator newend=std::remove_if(results_.begin(),results_.end(),isunmeasured);
        int cn=results_.end()-newend;
        results_.erase(newend,results_.end());
        return cn;
    }

    // -----------------------------------------------------------------------
    // 这个已经给你了：只读地拿到内部数据（测试要用它检查你的裁剪结果）
    // 注意它返回的是 const 引用，所以外面拿不到能改的权限。
    // -----------------------------------------------------------------------
    const std::vector<DetResult>& results() const { return results_; }

private:
    std::vector<DetResult> results_;      // 成员变量：类自己持有的数据
};

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

    // ---- 题 1：构造函数 ----
    std::cout << "\n[题 1] 构造函数\n";
    {
        DetFrame f1;                                   // 默认构造
        check(f1.size() == 0, "默认构造出来是空的");
        check(f1.empty(),     "默认构造 empty() 为真");

        std::vector<DetResult> src;
        src.push_back(mkResult(0, 0.9f, 100, 200, 180, 340, 3.5f));
        src.push_back(mkResult(2, 0.8f, 400, 210, 470, 330, 8.0f));
        DetFrame f2(src);                              // 用一组结果构造
        check(f2.size() == 2,                    "带参构造复制了 2 条");
        check(f2.size() == 2 && f2.results()[0].cls_id == 0, "第 1 条是行人");
        check(f2.size() == 2 && f2.results()[1].left == 400, "第 2 条的 left 是 400");
        check(src.size() == 2,                   "原 vector 没被搬空");
    }

    // ---- 题 2：addResult / size / empty ----
    std::cout << "\n[题 2] addResult / size / empty\n";
    {
        DetFrame f;
        check(f.empty(), "刚建好是空的");
        f.addResult(mkResult(0, 0.9f, 100, 200, 180, 340, 3.5f));
        check(f.size() == 1, "加了 1 条");
        check(!f.empty(),    "不再是空的");
        f.addResult(mkResult(2, 0.8f, 400, 210, 470, 330, 8.0f));
        check(f.size() == 2,                 "加了 2 条");
        check(f.size() == 2 && f.results()[1].cls_id == 2, "第 2 条是汽车，顺序对");
    }

    // ---- 题 3：countByClass ----
    std::cout << "\n[题 3] countByClass\n";
    {
        DetFrame f;
        f.addResult(mkResult(0, 0.9f, 100, 200, 180, 340, 3.5f));
        f.addResult(mkResult(2, 0.8f, 400, 210, 470, 330, 8.0f));
        f.addResult(mkResult(0, 0.7f,  50, 300, 120, 400, 1.2f));

        check(f.countByClass(0) == 2,  "2 个行人");
        check(f.countByClass(2) == 1,  "1 辆汽车");
        check(f.countByClass(99) == 0, "没有的类别返回 0");

        DetFrame e;
        check(e.countByClass(0) == 0,  "空帧返回 0");

        // 【关键】下面这一条走的是 const 引用。
        // 如果你的 countByClass 忘了写 const，这一行【编译不过】，
        // 报错是：passing 'const DetFrame' as 'this' argument discards qualifiers
        const DetFrame& cref = f;
        check(cref.countByClass(0) == 2, "const 对象也能调（证明 const 标对了）");
    }

    // ---- 题 4：findNearestIndex ----
    std::cout << "\n[题 4] findNearestIndex\n";
    {
        DetFrame f;
        f.addResult(mkResult(0, 0.9f, 100, 200, 180, 340,  3.5f));
        f.addResult(mkResult(2, 0.8f, 400, 210, 470, 330,  8.0f));
        f.addResult(mkResult(0, 0.7f,  50, 300, 120, 400,  1.2f));
        check(f.findNearestIndex() == 2, "最近的是下标 2（1.2 米）");

        DetFrame g;
        g.addResult(mkResult(0, 0.9f, 100, 200, 180, 340, -1.0f));
        g.addResult(mkResult(0, 0.8f, 400, 210, 470, 330,  2.0f));
        check(g.findNearestIndex() == 1, "跳过未测距的，返回下标 1");

        DetFrame h;
        h.addResult(mkResult(0, 0.9f, 100, 200, 180, 340, -1.0f));
        check(h.findNearestIndex() == -1, "全都没测距，返回 -1");

        DetFrame e;
        check(e.findNearestIndex() == -1, "空帧返回 -1");
    }

    // ---- 题 5：clampToImage ----
    std::cout << "\n[题 5] clampToImage\n";
    {
        DetFrame f;
        f.addResult(mkResult(0, 0.9f, -20, -30, 100, 120, 3.0f));
        f.addResult(mkResult(0, 0.8f, 600, 400, 700, 500, 2.0f));
        f.addResult(mkResult(0, 0.7f, 100, 100, 200, 200, 1.0f));

        f.clampToImage(640, 480);
        // 每条都先 f.size() == 3 &&，靠短路保护：
        // addResult 还是 TODO 时 frame 是空的，不先判长度就会越界崩掉，
        // 后面的测试你就一条都看不到了。
        check(f.size() == 3 && f.results()[0].left == 0,     "负的 left 抬到 0");
        check(f.size() == 3 && f.results()[0].top == 0,      "负的 top 抬到 0");
        check(f.size() == 3 && f.results()[0].right == 100,  "没越界的不动");
        check(f.size() == 3 && f.results()[1].right == 640,  "超出的 right 压到 640");
        check(f.size() == 3 && f.results()[1].bottom == 480, "超出的 bottom 压到 480");
        check(f.size() == 3 && f.results()[2].left == 100
                            && f.results()[2].bottom == 200, "范围内完全不动");
        check(f.size() == 3 && std::fabs(f.results()[1].depth - 2.0f) < 1e-6f,
              "depth 不许被改动");
        check(f.size() == 3, "没有增删元素");
    }

    // ---- 题 5 追加：整个框跑到画面外（day05 你栽过的地方）----
    std::cout << "\n[题 5-追加] 整个框在画面外\n";
    {
        DetFrame f;
        f.addResult(mkResult(0, 0.9f, 700, 10, 750, 50, 3.0f));   // 整框在右边外面
        f.clampToImage(640, 480);
        check(f.size() == 1 && f.results()[0].left == 640,
              "先裁再比较：left 也被压到 640");
        check(f.size() == 1 && f.results()[0].right - f.results()[0].left == 0,
              "宽度不是负数（用 day05 的 if 写法这里会是 -60）");
    }

    // ---- 题 6：removeUnmeasured ----
    std::cout << "\n[题 6] removeUnmeasured\n";
    {
        DetFrame f;
        f.addResult(mkResult(0, 0.9f, 100, 200, 180, 340,  3.5f));
        f.addResult(mkResult(0, 0.8f, 400, 210, 470, 330, -1.0f));   // 删
        f.addResult(mkResult(2, 0.7f,  50, 300, 120, 400,  1.2f));
        f.addResult(mkResult(0, 0.6f, 300, 220, 350, 300, -1.0f));   // 删

        int removed = f.removeUnmeasured();
        check(removed == 2,        "删掉了 2 个");
        check(f.size() == 2,       "长度真的变小了");
        check(f.size() == 2 && f.results()[0].left == 100, "第 1 个还在原地");
        check(f.size() == 2 && f.results()[1].cls_id == 2, "顺序没乱，剩下的是汽车");

        DetFrame all;
        all.addResult(mkResult(0, 0.9f, 100, 200, 180, 340, -1.0f));
        check(all.removeUnmeasured() == 1 && all.empty(), "全删光，变成空帧");

        DetFrame none;
        check(none.removeUnmeasured() == 0, "空帧返回 0");
    }

    std::cout << "\n==========================================\n";
    std::cout << "  通过 " << g_pass << " / 失败 " << g_fail << "\n";
    if (g_fail == 0) std::cout << "  全绿 —— 类这一关过了\n";
    std::cout << "  别忘了看警告：这一关要求【零警告】\n";
    std::cout << "==========================================\n";
    return 0;
}
