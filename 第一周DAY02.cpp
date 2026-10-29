第一周DAY02（5题）
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
错因（我的解题思路）：先遍历字符串，找出出现次数最少的字母，然后整体输出相应次数的"GPLT"，最后按顺序把每个字母剩余次数输出。但是对于动态数组vector、字符串string、字符char之间的关系和区别还分不清，且对str.size()与str.length()的用法和区别比较模糊。
注：在定义字符串时，未提前划分内存，则无法直接访问字符串中的元素。例如：string str;str[0]='a';这样的写法是错误的，str未划分大小，不会自动扩容，包括vetor也是不会自动扩容，写入前必须保证空间已存在，否则就用 push_back。例如：string str;str.push_back('a');这种写法是可行的，同时对于''和""的使用也需要注意。
    对于push_back(),一次只能添加一个元素，且字符串的push_back()只能添加字符（用''表示），例如'a'；不能添加字符串字面量（用""表示），例如"G"。
    如果想往字符串后面拼接字符串，不能使用push_back()，需要用+=或者append();例如ans+="PTL";或者ans.append("PTL");
    而对于ans+='a';和ans+="a";的区别：+'a'属于加一个char,只能加一个字节，如果+'PL'就会报错；+"a"属于加一个const char*（字符串字面量），可以实现+"PL"，加字符串字面量之前会先用strlen算一遍所加字符串的长度，再加上字符串，而加字符就不会计算长度，因为只能加一个字节。
    '\0'一个为0的字符，"\0"一个为空的字符串字面量，C++ 规定每个字符串字面量在结尾自动补一个 '\0'。所以 "\0" 实际是两个 NUL 字节。而 '\0' 就只有一个字节。"" 也占 1 字节（就那个隐式终止符），所以 "" 和 "\0" 在内存里字节内容完全一样，都为空，只是 sizeof 一个是 1 一个是 2。
    strlen是"数到第一个 \0 为止"，而 "\0" 的第一个字节就是 \0，所以数出来是 0。于是：sizeof('\0') = 1，sizeof("\0") = 2。strlen("\0") = 0，和 strlen("") 一样。
    注意s+="\0"和s+='\0'结果完全不同：前者什么都不加，后者加一个字节。因为 += 对 const char* 是走 strlen 的，对 char 是直接 push。例如：
       s += "\0";            // 追加 0 个字符（strlen 为 0，什么也没加）
       s += '\0';            // 真的追加 1 个 NUL 字节，字符串长度 +1
       s.push_back('\0');    // 同上，真的 +1 字节
       string ans = {'\0'};  // 长度变成 1，内容是一个 NUL 
    用到string的函数比如push_back()需要加头文件<string>,用到vector的函数需要加头文件<vector>。
    str.size()和str.length()含义几乎一样，推荐使用str.size()，表示统计字符串中的字符个数，因为其他比如vector、map也可以使用size()来计算包含元素数量，统一写法比较好。但是size() 返回的是无符号类型，所以：
      for (int i = 0; i < str.size(); i++)       // ⚠️ 会有 signed/unsigned 比较警告
      for (size_t i = 0; i < str.size(); i++)   // ✅ 推荐
    对于比较大小的min()与max()函数，一次只能比较两个元素，如果需要比较多个元素，需要嵌套或者初始化列表，例如：min(min(a,b),min(c,d))或者min({a,b,c,d})，且用到min和max函数需要添加头文件<algorithm>。

5.PAT"PAT (Basic Level) Practice"第1009题 说反话https://pintia.cn/problem-sets/994805260223102976/exam/problems/type/7?problemSetProblemId=994805314941992960
错因：未注意到用cin输入字符串，如果碰到空格会截停；跟上一题一样，没给字符串定义内存大小就用下标[]访问字符串元素；对empty()和clear()函数的作用模糊；以及分不清'\0'与' '的区别。
注：由于语言中默认cin>>a>>b;的a、b之间用空格来做区别，所以如果有char a;string b;那么cin>>a;cin>>b;之间若有空格，就会立即截停。要是想读取一整个输入内容，就需要读取一整行。
    getline()是std::string的配套函数，包含在头文件<string>中；本题使用string str;getline(cin,str);表示的意思是从cin中整行抓存入str中包括空格；getline()含糊的使用是括号内前一个表示从哪里抓取，后一个表示抓取了存放到哪里。对于getline()函数的使用方法还有很多，但是比较繁杂且内容较多，后续练习时碰到再详细记忆。
    '\0'是字符串结束符，不是空格，是NUL字节，在中间打空格（' '）才是空格字符。
    ''的写法是错的，会报错，字符必须占一个字符，不能什么也没有，如果想表达为空字符需要用'\0'（数值为0，长度为1），包括像'ab'也是不允许的，因为单引号字符只允许包含一个字符；' '这样才表达字符是空格（数值为32，长度为1）；
    ""的写法是允许的，表示字符串为空（长度为0），" "加空格表示含一个空格的字符串（长度为1），" "是占长度的，比如"a b"的size()就是3；
    empty()函数返回的是“是不是空”，如果返回true，则说明为空；clear()函数作用是清空；两函数不止作用于字符串，例如vector、deque、list、map、set也有这两个函数，且作用相同，但原生数组如int num[100]不能使用这两个函数。







       

       
