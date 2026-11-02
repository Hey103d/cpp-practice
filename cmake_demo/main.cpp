// ============================================================
// main.cpp  ——  主程序
// 它不认识 detector.cpp 里面写了什么，只认识 detector.h 的声明。
// 把两者真正连起来的是 CMake 里那一行 target_link_libraries
// ============================================================
#include <iostream>
#include <vector>
#include "detector.h"

int main()
{
    const int img_w = 640;
    const int img_h = 480;

    std::cout << "【1】图片尺寸 " << img_w << " x " << img_h << "\n";

    std::vector<Box> persons = detect_persons(img_w, img_h);
    std::cout << "【2】检测到 " << persons.size() << " 个人\n";

    for (std::size_t i = 0; i < persons.size(); ++i) {
        const Box& b = persons[i];
        std::cout << "     第 " << (i + 1) << " 个人: 坐标["
                  << b.left << "," << b.top << ","
                  << b.right << "," << b.bottom << "]"
                  << "  面积 " << box_area(b) << " 像素\n";
    }

    std::cout << "\n你能看到这几行，说明三件事都成了：\n";
    std::cout << "  1. main.cpp 编过了\n";
    std::cout << "  2. detector.cpp 被编成库了\n";
    std::cout << "  3. 库成功链接进来了\n";
    std::cout << "这三件事，全靠 CMakeLists.txt 里那几行命令。\n";
    return 0;
}
