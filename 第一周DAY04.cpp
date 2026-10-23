第一周DAY04（5题）
主要任务：分支结构（if/else if/switch）
预计练习：8 道题：成绩分级、三角形判断、菜单选择器

1.洛谷P5717 三角形分类https://www.luogu.com.cn/problem/P5717#ide
错因：注意细节，单词拼写输出容易输错；对max()函数的应用掌握的不够好；思路是正确的，但是代码写起来太复杂了，一直if函数嵌套很容易出错。
新解法：先把输入的a,b,c排序，保证a≤b≤c,那么判断是否构成三角形只用一个等式a+b>c就行，然后判断直角、锐角、钝角归为一个if类，判断等腰和等边归为一个if类。
注：max()和min()函数都包含在头文件<algorithm>中，要初始化列表调用这两个函数必须要加上对应的头文件，否则会报错；
   排序可以逐个用swap()函数对比交换，比如if(a>b)swap(a,b);但要注意的是要加头文件<algorithm>才能调用swap()函数；
   还可以用sort数组排序，比如cin>>a>>b>>c;int s[3]={a,b,c};sort(s,s+3);a=s[0];b=s[1];c=s[2];调用sort数组需要两个参数，起点位置和终点位置。s是数组名，在表达式里自动指向s[0]，s + 3 是“往前走 3 个位置”，但包含区间是左闭右开[s,s+3)；sort()函数也包含在有文件<algorithm>中，调用时需要添加头文件；
   if-else函数的用法：if(){...}else if(){...}else{...}必须对比的是同一个东西，比如if(a>b){...}else if(a==b){...}else{...}，如果出现if(a>b){...}else if((a*a)>c){...}else{...}则else if分支不会进行下去，因为他们对比的不是同一类型，else和else if必须属于同一分支，否则另开一个if分支，如if(a>b){...}if((a*a)>c){...}else{...}。

2.洛谷U158342 成绩等级判定https://www.luogu.com.cn/problem/U158342#ide
解题思路：if-else if 函数解答即可

3.洛谷P5714 肥胖问题https://www.luogu.com.cn/problem/P5714#ide
错因：又是输出单词漏字母问题，还是需要细心。
注：cout默认输出6位有效数字，且省掉后缀0。

4.洛谷P5716 月份天数https://www.luogu.com.cn/problem/P5716#ide
解题思路：先进行闰年判断，然后用switch嵌套进行输出。
注：每个case末端如果没有break就不会停止，会继续往下输出，包括嵌套switch函数后也需要加break才会中断，例如：
    switch(m){
        case 1:
            cout<<"31";
            break;
        case 2:
            switch(a){
                case 1:
                    cout<<"29";
                    break;
                case 2:
                    cout<<"28";
                    break;
            }
            break;     //如果这里不加break会继续输出case 3的内容
        case 3:
            cout<<"31";
            break;
   }

5.洛谷P5715 三位数排序https://www.luogu.com.cn/problem/P5715#ide
解题思路：与第一题先给输入的值排序一样，用swap()函数最快，但要注意添加头文件<algorithm>
         也可以用if函数嵌套，但是代码太长容易出错。




