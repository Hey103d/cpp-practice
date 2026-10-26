第二周DAY01
主要任务：函数：定义/声明/参数/返回值、值传递 vs 引用传递、默认参数、函数重载、递归入门。
预计练习：手写 5 个函数：素数判断 / 最大公约数 / 字符串反转 / 数组求和 / 阶乘(递归)。

1.洛谷P5705 数字反转https://www.luogu.com.cn/problem/P5705#ide
注：可以输入字符串，然后反向遍历输出；
    也可以用reverse()函数将两头对调，最前面的和最后的调换，然后往里进格，一直换到中间，与sort()函数相同，区间是左闭右开的；
    reverse(s.begin(), s.end())                                       与sort()函数都包含在头文件<algorithm>中
            ↑ 第一个字符  ↑ 最后一个字符的后一格

2.洛谷P5706 再分肥宅水https://www.luogu.com.cn/problem/P5706#ide
注：如果设置了cout<<fixef<<setprecision(3)<<a;此时该对浮点数的设置会一直保留在cout中，但如果输出的是int整型就不受影响，如果要取消对浮点数的设置，可以用cout.unsetf(ios::fixed);     // 老写法，任何编译器都认   或者    cout << defaultfloat;        // C++11 起，洛谷可用；Dev-C++ 老版本可能报错

3.洛谷P5708 三角形面积https://www.luogu.com.cn/problem/P5708#ide
注：开根号用sqrt()函数，包含在头文件<cmath>中
