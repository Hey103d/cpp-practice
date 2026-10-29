# cpp-practice

C++ 学习记录 —— 为本科毕设《基于 RK 系列平台的双目视觉行人定位与碰撞预警系统》打基础。

目标平台：RK3568 + RKNN YOLOv8 + 双目测距 + Qt。
学习方式：每天一个主题，写「概念演示」+「盲写练习」，用断言自测。

## 进度

| 日期 | 主题 | 文件 | 断言 | 状态 |
|---|---|---|---|---|
| 10/26 | 五个基础函数（素数/约数/反转/求和/阶乘） | `day01_盲写模板.cpp` | 27 | ✅ 27/27 |
| 10/27 | 指针与引用（交换/遍历/strlen/原地反转） | `day02_指针_盲写练习.cpp` | 25 | ✅ 25/25 |
| 10/27 | std::string 常用操作 | `day02b_字符串补课.cpp` | 14 | ✅ 14/14 |
| 10/28 | 结构体（初始化/`->`/三种传参/对齐） | `day03_结构体_盲写练习.cpp` | 26 | ✅ 26/26 |
| 10/28 | 结构体数组与嵌套（对标 `yolov8.h`） | `day04_结构体数组_练习.cpp` | 31 | ✅ 31/31 |
| 10/29 | std::vector 替代裸数组 | `day05_vector_练习.cpp` | 29 | 进行中 |

累计 **123 个断言通过**（day01–day04）。

## 目录说明

```
.
├── dayXX_*.cpp          每日练习（含测试与断言）
├── docs/
│   ├── log/             每日复盘日志
│   └── Git入门_从装到push.md
└── output/              编译产物（不入库）
```

## 编译与运行

```bash
g++ -std=c++17 -Wall -Wextra -Wshadow -O2 dayXX_xxx.cpp -o app.exe
./app.exe
```

固定开启 `-Wall -Wextra -Wshadow`：这三个开关抓到过变量遮蔽、多字符字面量、有符号/无符号比较等真实 bug。

## 与毕设的对应关系

| 练习中的东西 | 毕设里对应的东西 |
|---|---|
| `DetResult`（cls_id / prop / left / top / right / bottom / depth） | `yolov8.h` 的 `object_detect_result` |
| `DetResultList`（count + results[64]） | `object_detect_result_list` |
| `isOverlapping()` / `countNearby()` | 碰撞预警的判定逻辑 |
| `clampToImage()` | 检测框越界裁剪 |
| `depth` 字段 | 双目 SGBM 测距结果 |

## 说明

练习中的「盲写」指先不看参考实现自己写，写完用断言自测。参考答案保留在本地，不入库，避免直接照抄。
