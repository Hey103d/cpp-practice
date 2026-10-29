// day03_结构体_盲写练习.cpp
// 6 题 / 26 个断言 —— 每一题都是毕设里真正要写的东西
//
// 规则：
//   1. 只改 TODO 下面的函数体，别动 main 里的断言
//   2. 每写完一题就按 Ctrl+Shift+B 跑一遍，看通过数从多少涨到多少
//   3. 卡住超过 8 分钟就看一眼题目上的【提示】，还不行再来问
//
// 编译：g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day03_结构体_盲写练习.cpp -o day03p.exe

#include <iostream>
#include <string>
#include <cmath>

struct DetResult {
    int   cls_id;                    // 0=person  2=car
    float prop;                      // 置信度
    int   left, top, right, bottom;  // 框边界
    float depth;                     // 米；-1 表示还没测距
};

// ---------------------------------------------------------------------------
// 题 1：造一个检测结果
// 输入 6 个值，返回一个填好的 DetResult。depth 固定填 -1（表示还没测距）。
// 【提示】先 DetResult d{}; 再逐个赋值，最后 return d;
// ---------------------------------------------------------------------------
DetResult makeDetResult(int cls_id, float prop, int l, int t, int r, int b) {
    DetResult d{};
    // TODO
   d.cls_id=cls_id;
   d.prop=prop;
   d.left=l;
   d.top=t;
   d.right=r;
   d.bottom=b;
   d.depth=-1;
    return d;
}

// ---------------------------------------------------------------------------
// 题 2：框的宽度 = right - left
// 【提示】参数是 const 引用，直接用 d.right 就行
// ---------------------------------------------------------------------------
int boxWidth(const DetResult& d) {
    // TODO
    int wid=d.right-d.left;
    return wid;
}

// ---------------------------------------------------------------------------
// 题 3：框的面积 = 宽 × 高
// 注意这题参数是【传值】，先想一下：这和题 2 的 const 引用差在哪？
// 【提示】可以直接调用 boxWidth(d)
// ---------------------------------------------------------------------------
int boxArea(DetResult d) {
    // TODO
   
    int hei=d.bottom-d.top;
    int wid=boxWidth(d);
    int area=hei*wid;
    return area;
}

// ---------------------------------------------------------------------------
// 题 4：给检测结果写入深度
// 【提示】参数是指针，用 d->depth 写；写之前一定先判 d 是不是 nullptr
// ---------------------------------------------------------------------------
void setDepth(DetResult* d, float z) {
    // TODO
    if(d==nullptr)return;
   d->depth=z;
}

// ---------------------------------------------------------------------------
// 题 5：判断两个框是否重叠（这是碰撞预警的第一步）
// AABB 判重叠：只在【四个方向都没分开】时才算重叠
//   左右分开：a.right <= b.left  或  b.right <= a.left
//   上下分开：a.bottom <= b.top  或  b.bottom <= a.top
// 【提示】写成"没分开"的条件用 && 连起来；边界刚好贴住不算重叠
// ---------------------------------------------------------------------------
bool isOverlapping(const DetResult& a, const DetResult& b) {
    // TODO
   if(a.left<b.right&&b.left<a.right){
        if(a.top<b.bottom&&a.bottom>b.top){
            return true;
        }
   }
    return false;
}

// ---------------------------------------------------------------------------
// 题 6：统计【危险行人】个数（碰撞预警的核心逻辑）
// 条件：① cls_id == 0（只算行人）② depth >= 0（测出了距离）
//       ③ depth < threshold（比阈值近）
// 【提示】arr 可能为 nullptr、n 可能为 0，先挡住；再 for 遍历
// ---------------------------------------------------------------------------
int countNearby(const DetResult* arr, int n, float threshold) {
    // TODO
    int cn=0;
    if(n<=0||arr==nullptr)return cn;
    for(const DetResult* p=arr;p<arr+n;p++){
        if((*p).cls_id==0){
            if((*p).depth>=0&&(*p).depth<threshold){
                cn++;
            }
        }
    }
    return cn;
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
    std::cout << "\n[题 1] makeDetResult\n";
    {
        DetResult r = makeDetResult(2, 0.87f, 10, 20, 110, 220);
        check(r.cls_id == 2,                     "cls_id 被正确写入");
        check(r.left == 10,                      "left 被正确写入");
        check(r.bottom == 220,                   "bottom 被正确写入");
        check(std::fabs(r.prop - 0.87f) < 1e-6f, "prop 被正确写入");
        check(std::fabs(r.depth + 1.0f) < 1e-6f, "depth 是 -1（不是 0）");
    }

    // ---- 题 2 ----
    std::cout << "\n[题 2] boxWidth\n";
    {
        DetResult a{0, 0.9f, 100, 200, 180, 340, 3.5f};
        check(boxWidth(a) == 80,  "100~180 的宽度是 80");
        DetResult z{0, 0.9f, 0, 0, 0, 0, -1.0f};
        check(boxWidth(z) == 0,   "零宽框返回 0");
        DetResult bad{0, 0.9f, 640, 0, 100, 100, -1.0f};
        check(boxWidth(bad) == -540, "左右颠倒的脏数据会算出负数（提醒你要 clamp）");
    }

    // ---- 题 3 ----
    std::cout << "\n[题 3] boxArea\n";
    {
        DetResult a{0, 0.9f, 100, 200, 180, 340, 3.5f};
        int ar = boxArea(a);
        check(ar == 80 * 140,   "80x140 的面积是 11200");
        check(a.left == 100,    "传值调用之后，原结构体没被改动（面积也要算对）");
        DetResult z{0, 0.9f, 5, 5, 5, 5, -1.0f};
        check(boxArea(z) == 0,  "零面积返回 0");
    }

    // ---- 题 4 ----
    std::cout << "\n[题 4] setDepth\n";
    {
        DetResult a{0, 0.9f, 100, 200, 180, 340, -1.0f};
        setDepth(&a, 5.5f);
        check(std::fabs(a.depth - 5.5f) < 1e-6f && a.left == 100,
              "depth 写成 5.5，且别的字段没动");
        setDepth(nullptr, 3.0f);
        check(true,  "传 nullptr 不崩（说明你判空了）");
    }

    // ---- 题 5 ----
    std::cout << "\n[题 5] isOverlapping\n";
    {
        DetResult a{0, 0.9f, 100, 200, 200, 300, 3.0f};

        DetResult same{0, 0.9f, 100, 200, 200, 300, 3.0f};
        check(isOverlapping(a, same), "完全重合 → 重叠");

        DetResult half{0, 0.9f, 150, 250, 260, 360, 3.0f};
        check(isOverlapping(a, half), "部分交叉 → 重叠");

        DetResult inner{0, 0.9f, 120, 220, 160, 260, 3.0f};
        check(isOverlapping(a, inner), "小框套在大框里 → 重叠");

        DetResult cross{0, 0.9f, 50, 250, 150, 350, 3.0f};
        check(isOverlapping(a, cross), "左侧交叉 → 重叠");

        DetResult rightEdge{0, 0.9f, 200, 200, 300, 300, 3.0f};
        check(!isOverlapping(a, rightEdge), "右边刚好贴住 (a.right==b.left) → 不重叠");

        DetResult above{0, 0.9f, 100, 0, 200, 100, 3.0f};
        check(!isOverlapping(a, above), "上方刚好贴住 (b.bottom==a.top) → 不重叠");

        DetResult faraway{0, 0.9f, 500, 500, 600, 600, 3.0f};
        check(!isOverlapping(a, faraway), "隔很远 → 不重叠");
    }

    // ---- 题 6 ----
    std::cout << "\n[题 6] countNearby\n";
    {
        check(countNearby(nullptr, 5, 3.0f) == 0, "传 nullptr 返回 0");

        DetResult scene[5] = {
            {0, 0.9f, 100, 200, 180, 340,  1.5f},   // 行人 1.5m  → 算
            {0, 0.8f, 400, 210, 470, 330,  2.5f},   // 行人 2.5m  → 算
            {0, 0.7f,  50, 300, 120, 400,  4.0f},   // 行人 4.0m  → 不算（太远）
            {2, 0.9f, 600, 250, 700, 350,  1.0f},   // 汽车 1.0m  → 不算（不是行人）
            {0, 0.6f, 300, 220, 350, 300, -1.0f},   // 行人 没测距 → 不算
        };
        check(countNearby(scene, 5, 3.0f) == 2, "阈值 3m：2 个危险行人");
        check(countNearby(scene, 5, 1.6f) == 1, "阈值 1.6m：只剩 1 个");
        check(countNearby(scene, 5, 9.0f) == 3, "阈值 9m：3 个行人（仍排除未测距的）");
        check(countNearby(scene, 1, 3.0f) == 1, "只传 n=1 时只看第 1 个");
        check(countNearby(scene, 0, 3.0f) == 0, "n=0 返回 0");
    }

    std::cout << "\n==========================================\n";
    std::cout << "  通过 " << g_pass << " / 失败 " << g_fail << "\n";
    if (g_fail == 0) std::cout << "  全绿，今天这关过了。去提交选题表。\n";
    std::cout << "==========================================\n";
    return 0;
}
