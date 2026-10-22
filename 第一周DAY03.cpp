第一周DAY03
主要任务：常量、运算符、表达式、类型转换
预计练习：8 道题：闰年判断、奇偶判断、大小写转换、简易计算器

1.洛谷P5711 闰年判断https://www.luogu.com.cn/problem/P5711#ide
注：输出cout和返回return注意区别

2.洛谷B2037 奇偶数判断https://www.luogu.com.cn/problem/B2037#ide
进阶：洛谷B2035 判断数正负https://www.luogu.com.cn/problem/B2035#ide
注：int/long/long long 定义的数值范围：int一般为±2.1*10^9、long long一般为±9.2*10^18、long的话各平台（windows、mac、linux）差异比较大，不推荐使用

3.洛谷B4556 字符转换https://www.luogu.com.cn/problem/B4556#ide
错因：对于强制转换与运算符的优先级考虑不当以及逐个输出和字符串整体输出的运用不当，大小写字母的转换不清晰
注：字符c若是字母，小写字母在数值上会比大写字母多32。大写A~Z的取值范围是65~90，小写a~z的取值范围是97~122。
    如果有char c='a'-32;得到的是字符'A',因为'a'-32得到的int被强制转换成了字符并赋值给c，单如果像char c;cout<<c-32;输出的就是数值了，因为c-32最后得到的结果是int型，需要输出字符就要强制转换cout<<(char)(c-32);注意这里的强制转换（char)优先级比（-）更高，如果写成cout<<(char)c-32的话本质上还是cout<<((char)c)-32，最后输出的还是数值。
    也可以用tolower()或者toupper()函数来进行大小写转换，但要注意的是使用的对象要求是无符号字符，要先把需要转换的字符强制转换成unsigned char类型，而且值得注意的是函数返回的值是int类型，如果需要输出字符，则还需要再进行强制转换，例如：
         unsigned char u = (unsigned char)c;
         cout<<(char)tolower(u);
    并且这两个函数不管数字和其他字符，只管字母，例如：
         tolower('5') = 5      // 数字原样返回，不会变成 *
         toupper('*') = *      // 非字母也是原样返回
    两个函数跟isdigit()（判断是否是0~9的数字）、islower()（判断是否是小写字母）、isupper()（判断是否是大写字母）都包含在头文件<cctype>头文件中，且这三个函数的使用对象也需要是unsigned char类型，返回值也是int类型，返回值非零即真。
    对于遍历字符串中的每个字符for(char c:str),逐个输出每个字符占用内存过大且耗时过长，不推荐；更好的办法是定义一个新的字符串ans，把转换后的字符先存入ans，最后再整体输出字符串ans耗时会更小，占用内存会更少；但其实二者本质上的区别并不大。

4.洛谷B2052 简单计算器https://www.luogu.com.cn/problem/B2052#ide



