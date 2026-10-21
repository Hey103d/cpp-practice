第一周DAY02
主要任务：变量与数据类型（int/double/char/bool/string）
预计练习类型：8 道题：温度转换、圆面积、字符判断、字符串拼接等

完成习题：
1.洛谷P1001 A+B问题https://www.luogu.com.cn/problem/P1001#ide
注：输入流运算符和输出流运算符要记清楚

2.洛谷B2013 温度表达转化https://www.luogu.com.cn/problem/B2013#ide
注：认识标准库<iomanip>里的新函数setprecision(),设置有效数字位数，例如cout<<setprecision(2)<<y;为输出y且保留2位有效数字
    如果需要保留小数点后的位数，则搭配fixed使用，例如cout<<fixed<<setprecision(5)<<x;为输出x保留小数点后5位
    但是要注意使用setprecision()时要记得#include<iomanip>
    另外，注意double和float的使用范围：double有效数字约15~16位，8字节；float有效数字约6~7位，4字节
    浮点数常见坑：float字面量要加f（float x=3.14f)、不要用 == 比浮点数、float做累加容易误差爆炸
    一般情况下直接用double比较保险，以后如果遇到数据量较大或内存不足的问题再考虑用float

3.洛谷B2014 与圆相关的计算https://www.luogu.com.cn/problem/B2014#ide
注：fixed和setprecision()具有粘性，设置一次之后，只要不修改无需再次设置
   如果想取消，先改回默认精度，再取消，例如：
fixedstreamsize old_prec = cout.precision();   // 保存当前精度
cout << fixed << setprecision(2) << a << endl;
cout.precision(old_prec);                 // 恢复
cout.unsetf(ios::fixed);                  // 取消 fixed
   若需中途修改保留的小数位数，重新设置setprecision()即可，fixed只要不取消依旧具有粘性，例如：
cout << fixed << setprecision(4) << a << " "<< setprecision(2) << b << " " << setprecision(4) << c;     // a=4位，b=2位，c=4位
   粘性操作符：setprecision(n)、fixed/scientific、left/right（对齐方式）、setfill(c)、hex（16进制）/dec（十进制）/oct（八进制）
   非粘性操作符（设置后只管一次）：setw(n)、endl
   C++中全局定义一个数字常量，用例constexpr int a=1；（性能更好）或const int a=1;（#define a 1也能达到这样的效果，但是不推荐用，无法判断a的变量类型）
   全局常量一般用全大写，单词间用下划线连接
补充：hex（16进制）/dec（十进制）/oct（八进制）相关知识点
     大写十六进制：默认hex是小写字母，用uppercase变大写：cout << hex << uppercase << 255;   // FF
     显示进制前缀（0x、0）：用showbase：cout << showbase;
                                      cout << hex << 255 << endl;   // 0xff
                                      cout << oct << 255 << endl;   // 0377
                                      cout << dec << 255 << endl;   // 255  （十进制没有前缀）
     组合使用：cout << showbase << uppercase << hex << 255;   // 0XFF  
注：只对整数有效，浮点数不受影响（浮点数有自己的 hexfloat）。
    默认是dec，所以开头不写也行，但用了hex之后记得改回来。
    showbase对dec不显示前缀，这是规定（十进制数不带前缀）。

4.PAT天梯赛L1-023 输出GPLT https://pintia.cn/problem-sets/994805046380707840/exam/problems/type/7?problemSetProblemId=994805113036587008
