// ============================================================
// detector.cpp  ——  实现文件
// 这里写 detector.h 里那些函数的具体实现
// 这个文件会被 CMake 编成静态库 libdetector.a
// ============================================================
#include "detector.h"

std::vector<Box> detect_persons(int img_w, int img_h)
{
    std::vector<Box> res;

    Box a;
    a.left   = img_w / 10;
    a.top    = img_h / 2;
    a.right  = img_w / 4;
    a.bottom = img_h - 20;
    res.push_back(a);

    Box b;
    b.left   = img_w * 3 / 4;
    b.top    = img_h / 2;
    b.right  = img_w - 30;
    b.bottom = img_h - 40;
    res.push_back(b);

    return res;
}

int box_area(const Box& b)
{
    return (b.right - b.left) * (b.bottom - b.top);
}
