// day02_指针_盲写练习.cpp
// 今晚的作业：填完 5 个 TODO，跑到 "0 失败" 才算过
// 编译运行：g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day02_指针_盲写练习.cpp -o p && ./p
//
// 规则（和昨天一样）：
//   1. 不许打开 day02_指针_概念演示.cpp
//   2. 卡住 20 分钟以上再看，看完关掉重写
//   3. 每道题都问自己一句："我是在用下标，还是在用指针？"
#include <iostream>
#include <cstring>
#include <climits>
#include <string> 
#include <climits>

// ============================================================
// 题 1：swap —— 三个版本，体会"值传递"和"地址传递"的区别
// ============================================================

// 1a 已经写好了，作为对照：换不了，因为 a、b 是复制品
void swapByValue(int a, int b) {
	int t = a; a = b; b = t;
}

// 1b 你要写的：通过指针换
// 提示：形参是 int*，那么"本体"是 *a，交换的是 *a 和 *b，不是 a 和 b
void swapByPtr(int* a, int* b) {
	// TODO
	int t=*a;
	*a=*b;
	*b=t;
}

// 1c 你要写的：通过引用换（引用就是别名，写法最像值传递，但真的能换）
void swapByRef(int& a, int& b) {
	// TODO
	int t=a;
	a=b;
	b=t;
}

// ============================================================
// 题 2：用【指针】遍历数组求和（不许写 arr[i]）
// 提示：for (const int* p = arr; p < arr + n; ++p) s += *p;
// ============================================================
long long sumByPtr(const int* arr, int n) {
	// TODO
	long long sum=0;
	for(const int* p=arr;p<arr+n;++p){
		sum+=(long long)*p;
	}
	return sum;
}

// ============================================================
// 题 3：用【指针】找最大值（同样不许写 arr[i]）
// 约定：n <= 0 时返回 0
// 提示：max 初值取 *arr，然后 p 从 arr+1 开始走
// ============================================================
int maxByPtr(const int* arr, int n) {
	// TODO
	if(n>0){
		int max=*arr;
		for(const int *p=arr+1;p<arr+n;p++){
			if(*p>=max)max=*p;
		}
		return max;
	}
	else {return {0};}
}

// ============================================================
// 题 4：手写 strlen（不许调用 std::strlen）
// 提示：指针一直往前走，直到 *p == '\0'；最后 p - s 就是长度
// 注意：返回 size_t，空串要返回 0，不能崩
// ============================================================
size_t myStrlen(const char* s) {
	// TODO
	size_t length;
	if(*s=='\0')return 0;
	else{
		const char* p=s;
		while(*p!='\0'){
			p++;
		}
		length=p-s;
	}
	return length;
}

// ============================================================
// 题 5：原地反转 C 字符串（双指针，不申请新内存）
// 提示：i 从头走、j 从尾走（j 先指到 '\0' 再退一格），边走边换
// 注意：空串时 j 会退到 s 前面，必须防住
// ============================================================
void reverseInPlace(char* s) {
	// TODO
	int len;
	char* p=s;
	while(*p!='\0'){
		p++;
	}
	len=p-s;
	if(len<2)return;
	else{
		p--;
		for(char* i=s,*j=p;i<j;i++,j--){
				int t=*j;
				*j=*i;
				*i=t;
		}
		return ;
	}

}

// ------------------------- 测试代码，不要改 -------------------------
static int g_pass = 0, g_fail = 0;
void check(bool ok, const std::string& name) {
	if (ok) { ++g_pass; std::cout << "  [PASS] " << name << "\n"; }
	else    { ++g_fail; std::cout << "  [FAIL] " << name << "\n"; }
}

int main() {
	std::cout << std::unitbuf; 
	std::cout << "===== 1. swap =====\n";
	{
		int x = 1, y = 2;
		swapByValue(x, y);
		check(x == 1 && y == 2, "swapByValue 换不了（这是对的）");
	}
	{
		int x = 1, y = 2;
		swapByPtr(&x, &y);
		check(x == 2 && y == 1, "swapByPtr 换成功");
	}
	{
		int x = -7, y = 42;
		swapByPtr(&x, &y);
		check(x == 42 && y == -7, "swapByPtr 负数也能换");
	}
	{
		int x = 5, y = 9;
		swapByRef(x, y);
		check(x == 9 && y == 5, "swapByRef 换成功");
	}

	std::cout << "===== 2. sumByPtr =====\n";
	{
		int a[] = {1, 2, 3, 4, 5};
		check(sumByPtr(a, 5) == 15, "sum{1,2,3,4,5} = 15");
	}
	check(sumByPtr(nullptr, 0) == 0, "空数组 = 0");
	{
		int a[] = {-3, 7, -1, 0};
		check(sumByPtr(a, 4) == 3, "sum{-3,7,-1,0} = 3");
	}
	{
		int a[] = {2000000000, 2000000000};
		check(sumByPtr(a, 2) == 4000000000LL, "两个 20 亿相加不溢出");
	}
	{
		int a[100];
		for (int i = 0; i < 100; ++i) a[i] = i + 1;
		check(sumByPtr(a, 100) == 5050, "100 个元素 = 5050");
	}

	std::cout << "===== 3. maxByPtr =====\n";
	{
		int a[] = {3, 9, 2, 7};
		check(maxByPtr(a, 4) == 9, "max{3,9,2,7} = 9");
	}
	{
		int a[] = {-5, -1, -9};
		check(maxByPtr(a, 3) == -1, "全负数 max = -1");
	}
	{
		int a[] = {42};
		check(maxByPtr(a, 1) == 42, "单个元素");
	}
	{
		int a[] = {1, 2, 3, 4, 5};
		check(maxByPtr(a, 5) == 5, "最大值在末尾");
	}
	check(maxByPtr(nullptr, 0) == 0, "n<=0 返回 0");

	std::cout << "===== 4. myStrlen =====\n";
	check(myStrlen("") == 0, "空串 = 0");
	check(myStrlen("a") == 1, "\"a\" = 1");
	check(myStrlen("hello") == 5, "\"hello\" = 5");
	check(myStrlen("hello world") == 11, "含空格 = 11");
	check(myStrlen("中文") == 6, "\"中文\" = 6（UTF-8 每个汉字 3 字节）");
	{
		const char* s = "abcdefghij";
		check(myStrlen(s) == std::strlen(s), "和 std::strlen 结果一致");
	}

	std::cout << "===== 5. reverseInPlace =====\n";
	{
		char b[] = "hello";
		reverseInPlace(b);
		check(std::strcmp(b, "olleh") == 0, "hello -> olleh");
	}
	{
		char b[] = "ab";
		reverseInPlace(b);
		check(std::strcmp(b, "ba") == 0, "偶数长度 ab -> ba");
	}
	{
		char b[] = "a";
		reverseInPlace(b);
		check(std::strcmp(b, "a") == 0, "单字符不崩");
	}
	{
		char b[] = "";
		reverseInPlace(b);
		check(b[0] == '\0', "空串不崩（这里最容易越界）");
	}
	{
		char b[] = "Never odd or even";
		reverseInPlace(b);
		check(std::strcmp(b, "neve ro ddo reveN") == 0, "含空格的长串");
	}

	std::cout << "\n===== 结果：" << g_pass << " 通过, " << g_fail << " 失败 =====\n";
	return g_fail == 0 ? 0 : 1;
}
