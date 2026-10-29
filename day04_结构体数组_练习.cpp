// day04_结构体数组_练习.cpp
// 今天只学一件事：把 DetResult 装进"一帧的检测结果列表"里
//
// 为什么是这个：rknn_model_zoo 的 yolov8.h 里除了 object_detect_result，
// 还有一个 object_detect_result_list —— 就是下面这个 DetResultList 的原版。
// 会写它，你下周读那个头文件就只剩语法陌生，没有结构陌生。
//
// 编译：g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day04_结构体数组_练习.cpp -o day04.exe
//
// ===========================================================================
// 概念速览（先读这 30 行，再动 TODO）
// ===========================================================================
// 1) 结构体数组和普通数组没区别，只是每个元素是一个结构体
//      DetResult results[64];
//      results[0].left = 100;          // 先取元素，再用点取成员
//      results[i].depth = 3.5f;
//
// 2) 数组成员的指针要用箭头
//      DetResultList* p = &list;
//      p->count                        // 指针 -> 成员
//      p->results[0].cls_id            // 先 -> 进结构体，再用 [0] 取元素，再用 . 取成员
//
// 3) count 和 results 必须放在同一个结构体里，理由和昨天那道 nullptr 题一样：
//      如果写成两个独立参数 countNearby(arr, n)，
//      调用方就可能传 arr=nullptr 但 n=5。
//      绑进一个结构体后，"有几个"和"存在哪"永远一起传递，配不错。
//
// 4) 返回值是结构体的时候，别返回指针
//      DetResult  makeResult(...)     // 好：值返回，调用方随便改，互不影响
//      DetResult* makeResult(...)     // 坏：返回局部变量的地址，函数一结束就悬空

#include <iostream>
#include <string>
#include <cstring>
#include <cmath>

#define MAX_OBJ 64

struct DetResult {
    int   cls_id;                    // 0=person  2=car
    float prop;
    int   left, top, right, bottom;
    float depth;                     // 米；-1 表示还没测距
};

// 一帧的检测结果：就是 yolov8.h 里 object_detect_result_list 的等价物
struct DetResultList {
    int       count;                 // 实际检测到几个（0 ~ 64）
    DetResult results[MAX_OBJ];      // 最多存 64 个
};

// ---------------------------------------------------------------------------
// 题 1：把列表清空
// 要求：① list 为 nullptr 时直接返回 ② count 归 0 ③ results 全部清零
// 【提示】判空之后，memset(list, 0, sizeof(DetResultList)) 一行就够；
//         或者 list->count = 0; 再用一个循环把 results 清掉（你选一个）
// ---------------------------------------------------------------------------
void initList(DetResultList* list) {
    // TODO
    if(list==nullptr)return;
    memset(list,0,sizeof(DetResultList));
}

// ---------------------------------------------------------------------------
// 题 2：往列表里加一个检测结果
// 要求：① list 为 nullptr 返回 false
//       ② 已经满了（count >= MAX_OBJ）返回 false，不要越界写
//       ③ 成功则写进 results[count]，count 加 1，返回 true
// 【提示】写入是 list->results[list->count] = r; 然后 ++list->count;
// ---------------------------------------------------------------------------
bool addResult(DetResultList* list, const DetResult& r) {
    // TODO
    if(list==nullptr)return false;
    if(list->count>=MAX_OBJ)return false;
    list->results[list->count]=r;
    list->count++;
    return true;
}

// ---------------------------------------------------------------------------
// 题 3：数一数某个类别有几个
// 要求：统计 list 里 cls_id 等于传入值的元素个数
// 【提示】参数是 const 引用，循环 i 从 0 到 list.count - 1
// ---------------------------------------------------------------------------
int countByClass(const DetResultList& list, int cls_id) {
    // TODO
    int cn=0;
    for(int i=0;i<list.count;i++){
        if(list.results[i].cls_id==cls_id){
            cn++;
        }
    }
    return cn;
}

// ---------------------------------------------------------------------------
// 题 4：找出"最近的那个目标"的下标
// 要求：返回 depth 最小的那个元素的下标；depth 为 -1（未测距）的不参与比较。
//       如果列表为空、或者所有目标都没测距，返回 -1。
// 【提示】先设 best = -1 当"还没有候选"，再遍历：
//         遇到第一个有效 depth 就先记下它，之后只换成更小的
// ---------------------------------------------------------------------------
int findNearestIndex(const DetResultList& list) {
    // TODO
    int best=-1;
    for(int i=0;i<list.count;i++){
        if(list.results[i].depth<0)continue;
        if(best==-1||list.results[i].depth<list.results[best].depth){
            best=i;
        }
    }
    return best;
}

// ---------------------------------------------------------------------------
// 题 5：把所有框裁进画面内（这是真实必须做的一步）
// 要求：原地修改每个元素，让 0 <= left < right <= w 且 0 <= top < bottom <= h
//       越界就压到边界上，没越界的不要动。
// 【提示】参数是指针（要改原件），循环里用 list->results[i].left 这种写法
// ---------------------------------------------------------------------------
void clampToImage(DetResultList* list, int w, int h) {
    // TODO
    if(list==nullptr)return;
    for(int i=0;i<list->count;i++){
        if(list->results[i].right>w)list->results[i].right=w;
        if(list->results[i].left<0)list->results[i].left=0;
        
        if(list->results[i].top<0)list->results[i].top=0;
        if(list->results[i].bottom>h)list->results[i].bottom=h;
    }
}

// ===========================================================================
// 以下是测试，不要修改
// ===========================================================================
static int g_pass = 0, g_fail = 0;

void check(bool cond, const std::string& name) {
    if (cond) { ++g_pass; std::cout << "  [PASS] " << name << "\n"; }
    else      { ++g_fail; std::cout << "  [FAIL] " << name << "\n"; }
}

DetResult mk(int cls, float prop, int l, int t, int r, int b, float dep) {
    DetResult d{};
    d.cls_id = cls; d.prop = prop;
    d.left = l; d.top = t; d.right = r; d.bottom = b;
    d.depth = dep;
    return d;
}

int main() {
    std::cout << std::unitbuf;

    // ---- 题 1 ----
    std::cout << "\n[题 1] initList\n";
    {
        DetResultList L;
        L.count = 9;
        L.results[0] = mk(0, 0.9f, 1, 2, 3, 4, 5.0f);
        L.results[63] = mk(2, 0.8f, 1, 2, 3, 4, 5.0f);

        initList(&L);
        check(L.count == 0,                  "count 归 0");
        check(L.results[0].left == 0,        "results[0] 被清零");
        check(L.results[63].cls_id == 0,     "results[63] 被清零（最后一个也清了）");
        initList(nullptr);
        check(true,                          "传 nullptr 不崩");
    }

    // ---- 题 2 ----
    std::cout << "\n[题 2] addResult\n";
    {
        DetResultList L;
        initList(&L);

        check(addResult(&L, mk(0, 0.9f, 100, 200, 180, 340, 3.5f)) == true, "空表加第 1 个返回 true");
        check(L.count == 1,                    "count 变成 1");
        check(L.results[0].left == 100,        "第 1 个写进了 results[0]");

        check(addResult(&L, mk(2, 0.8f, 10, 20, 60, 90, 8.0f)) == true, "再加第 2 个返回 true");
        check(addResult(&L, mk(0, 0.7f, 30, 40, 70, 99, 1.2f)) == true, "再加第 3 个返回 true");
        check(L.count == 3,                    "count 变成 3");
        check(L.results[2].cls_id == 0,        "第 3 个写进了 results[2]");

        DetResultList full;
        initList(&full);
        for (int i = 0; i < MAX_OBJ; ++i) addResult(&full, mk(0, 0.5f, i, i, i + 1, i + 1, 1.0f));
        check(full.count == MAX_OBJ,           "连续加 64 个后 count = 64");
        check(addResult(&full, mk(0, 0.5f, 0, 0, 1, 1, 1.0f)) == false, "满了以后返回 false");
        check(full.count == MAX_OBJ,           "满了以后 count 不再增加");

        check(addResult(nullptr, mk(0, 0.5f, 0, 0, 1, 1, 1.0f)) == false, "传 nullptr 返回 false");
    }

    // ---- 题 3 ----
    std::cout << "\n[题 3] countByClass\n";
    {
        DetResultList L;
        initList(&L);
        addResult(&L, mk(0, 0.9f, 100, 200, 180, 340, 3.5f));   // 行人
        addResult(&L, mk(0, 0.8f, 400, 210, 470, 330, 1.2f));   // 行人
        addResult(&L, mk(2, 0.7f,  50, 300, 300, 420, 8.0f));   // 汽车
        addResult(&L, mk(0, 0.6f, 300, 220, 350, 300, -1.0f));  // 行人（没测距也算行人）

        check(countByClass(L, 0) == 3,    "3 个行人");
        check(countByClass(L, 2) == 1,    "1 辆汽车");
        check(countByClass(L, 99) == 0,   "没有的类别返回 0");

        DetResultList empty;
        initList(&empty);
        check(countByClass(empty, 0) == 0, "空表返回 0");
    }

    // ---- 题 4 ----
    std::cout << "\n[题 4] findNearestIndex\n";
    {
        DetResultList L;
        initList(&L);
        addResult(&L, mk(0, 0.9f, 100, 200, 180, 340,  3.5f));   // idx 0
        addResult(&L, mk(2, 0.8f, 400, 210, 470, 330,  8.0f));   // idx 1
        addResult(&L, mk(0, 0.7f,  50, 300, 120, 400,  1.2f));   // idx 2 最近
        check(findNearestIndex(L) == 2, "最近的是下标 2（1.2 米）");

        DetResultList L2;
        initList(&L2);
        addResult(&L2, mk(0, 0.9f, 100, 200, 180, 340, -1.0f));  // 没测距
        addResult(&L2, mk(0, 0.8f, 400, 210, 470, 330,  2.0f));
        check(findNearestIndex(L2) == 1, "跳过没测距的，返回下标 1");

        DetResultList L3;
        initList(&L3);
        addResult(&L3, mk(0, 0.9f, 100, 200, 180, 340, -1.0f));
        addResult(&L3, mk(2, 0.8f, 400, 210, 470, 330, -1.0f));
        check(findNearestIndex(L3) == -1, "全都没测距，返回 -1");

        DetResultList L4;
        initList(&L4);
        check(findNearestIndex(L4) == -1, "空表返回 -1");
    }

    // ---- 题 5 ----
    std::cout << "\n[题 5] clampToImage\n";
    {
        DetResultList L;
        initList(&L);
        addResult(&L, mk(0, 0.9f, -20, -30, 100, 120, 3.0f));    // 左上越界
        addResult(&L, mk(0, 0.8f, 600, 400, 700, 500, 2.0f));    // 右下越界
        addResult(&L, mk(0, 0.7f, 100, 100, 200, 200, 1.0f));    // 本来就在范围内

        clampToImage(&L, 640, 480);
        check(L.results[0].left == 0,      "负的 left 被抬到 0");
        check(L.results[0].top == 0,       "负的 top 被抬到 0");
        check(L.results[0].right == 100,   "没越界的 right 不动");
        check(L.results[1].right == 640,   "超出的 right 压到 640");
        check(L.results[1].bottom == 480,  "超出的 bottom 压到 480");
        check(L.results[2].left == 100 && L.results[2].bottom == 200, "范围内的框完全不动");
        check(L.results[1].depth == 2.0f,  "depth 不许被改动");
        clampToImage(nullptr, 640, 480);
        check(true,                        "传 nullptr 不崩");
    }

    std::cout << "\n==========================================\n";
    std::cout << "  通过 " << g_pass << " / 失败 " << g_fail << "\n";
    if (g_fail == 0) std::cout << "  全绿 —— 结构体这一块算通关了\n";
    std::cout << "==========================================\n";
    return 0;
}
