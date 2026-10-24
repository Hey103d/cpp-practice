第一周DAY05（5题）
主要任务：循环（for/while/do-while、break/continue）
预计练习：8 道题：九九乘法表、1–100 素数、水仙花数、阶乘求和

1.洛谷B3700 九九乘方表https://www.luogu.com.cn/problem/B3700#ide
解题思路：while、for、do...while函数的混合运用。

2.洛谷B2128 素数个数https://www.luogu.com.cn/problem/B2128#ide
错因：使用低级的for、if函数嵌套，并且一直循环遍历完整个比i小的数，太慢了导致超时。没有考虑到其实遍历到√i就行，因为√i之前如果存在i的因数，那一定与一个√i之后的数配对成为i的因数，后半部分就不用遍历了，且一旦找到第一个i的因数就不用再往后遍历了，因为这时候i就绝对不会是素数，然后用break立即结束遍历，减少时间。
注：break是能用在if函数中的，前提是if函数嵌套在循环函数中，如果if函数没有嵌套在循环函数中即单纯的if函数使用了break会报错。
   break只能使用在循环函数（for、while、do...while）和switch里，结束的是离他最近的一层循环，如果在if函数里用break，结束的就是if函数外最近的一层for或者while循环。

3.洛谷B2153 求阶乘的和https://www.luogu.com.cn/problem/B2153#ide
注：C++默认函数中不能再嵌套函数，一般在全局上定义函数，并且如果在主函数中先运用的函数是在主函数后定义的，需要在主函数前进行声明。

4.洛谷U18284 水仙花数https://www.luogu.com.cn/problem/U18284#ide
解题思路：可以利用（%）和（/）来解答；也可以用while和if函数配合解答，遍历输入数的每一位。

5.洛谷P5720 一尺之棰https://www.luogu.com.cn/problem/P5720#ide
注：“需要几天”和“在第几天”用词的区别。
