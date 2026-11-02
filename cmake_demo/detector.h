// ============================================================
// detector.h  ——  声明文件
// 作用：告诉别的 .cpp「有这么些函数可以用」
// 头文件里只写声明，不写具体实现
// ============================================================
#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>

// 一个框的四个边界（对标官方 rknn 的 box_rect_t）
struct Box {
    int left;
    int top;
    int right;
    int bottom;
};

// 假装检测出几个人（真程序里这里是 NPU 推理的结果）
std::vector<Box> detect_persons(int img_w, int img_h);

// 算框的面积
int box_area(const Box& b);

#endif
