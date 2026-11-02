# ============================================================
# CMakeLists.txt  就是整个工程的「编译说明书」
# 你只要看懂下面 7 块，就能读懂 rknn_model_zoo 里所有 CMakeLists
# ============================================================
#
# 用法（在本目录下敲两行）：
#   cmake -G "MinGW Makefiles" -B build
#   mingw32-make -C build
#
# 第一行叫「配置」：读这个文件，生成 makefile
# 第二行叫「编译」：真正调 g++ 把 .cpp 变成 .exe
# ============================================================


# ---------- 块 1：CMake 最低版本 ----------
# 板子上是 3.16.3，这里写 3.15，电脑和板子两边都能跑
cmake_minimum_required(VERSION 3.15)


# ---------- 块 2：工程名 ----------
# 只用于 IDE 显示，不影响任何东西
project(cmake_demo CXX)


# ---------- 块 3：C++ 标准 ----------
# 相当于你平时敲的 -std=c++17
# REQUIRED ON = 必须是 17，达不到就报错，别偷偷降级
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# ---------- 块 4：警告选项 ----------
# 相当于你平时敲的 -Wall -Wextra -Wshadow
add_compile_options(-Wall -Wextra -Wshadow)


# ---------- 块 5：头文件搜索路径 ----------
# 相当于 g++ 加 -I.
# 说实话：现在三个文件都在同一层目录，删掉这一行也能编过，
# 因为 #include "detector.h" 默认会先在「源文件自己所在的目录」里找。
# 但只要头文件挪进子目录（比如 include/detector.h），就必须靠这一行。
# rknn_model_zoo 里到处是这种写法，所以你要认得它。
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


# ---------- 块 6：把 detector.cpp 编成静态库 ----------
# 相当于手工敲：g++ -c detector.cpp  然后  ar rcs libdetector.a detector.o
# STATIC 表示静态库，产物叫 libdetector.a
# 这步不是必须的，也可以把 detector.cpp 直接写进 add_executable
# 但分开写更像 rknn_model_zoo（它把 utils 编成 imageutils 等几个库）
add_library(detector STATIC detector.cpp)


# ---------- 块 7：生成可执行文件 + 链接库 ----------
# 从 main.cpp 生成 cmake_demo.exe
add_executable(cmake_demo main.cpp)

# 把上面那个库接到 exe 上，相当于 g++ main.cpp -ldetector
# PRIVATE 先不用管，照抄就行
target_link_libraries(cmake_demo PRIVATE detector)
