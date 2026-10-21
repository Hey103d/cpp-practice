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
