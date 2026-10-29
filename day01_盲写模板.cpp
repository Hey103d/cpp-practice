// ============================================================
//  day01_盲写模板.cpp
//  W1 D1 · 盲写用。把 5 �? TODO 填完，编译运行，看到 "0 失败" 才算今天过关�?
//
//  编译：g++ -std=c++17 -Wall -Wextra -O2 day01_盲写模板.cpp -o blind
//  运行�?./blind
//
//  规则：不许打开 day01_five_functions.cpp。卡�? 20 分钟以上再看，看完关掉重写�?
// ============================================================

#include <iostream>
#include <string>
#include <climits>
#include <algorithm>

// 1. 素数判断：n < 2 返回 false；只需试除�? sqrt(n)
bool isPrime(int n) {
    // TODO: 在这里写你的实现
    if(n>=2){
        for(int i=2;i<=n/i;i++){
            if(n%i==0){
                return false;
            }
        }
        return true;
    }
    return false;
}

// 2. 最大公约数：辗转相除，循环�?
int gcd(int a, int b) {
    // TODO
    int c;
    if(a<0)a=-a;
    if(b<0)b=-b;
    if(a==0&&b!=0)return b;
    if(b==0&&a!=0)return a;
    if(a==0&&b==0)return 0;
    else{
        while(b>0){
            c=a%b;
            a=b;
            b=c;
        }
    }
    return a;
}

// 3. 字符串反转：原地，双指针，参数必须是引用 &
void reverseString(std::string& s) {
    // TODO
    for(int i=s.size()-1,j=0;i>j;i--,j++){
            int tmp=s[i];
            s[i]=s[j];
            s[j]=tmp;
    }
}

// 4. 数组求和：注意返回值比 int �?
long long sumArray(const int* arr, int n) {
    // TODO
    long long sum=0;
    for(int i=0;i<n;i++){
        sum+=(long long)arr[i];
    }
    return sum;
}

// 5. 阶乘：递归实现，注�? 0! = 1 和溢出的边界
unsigned long long factorial(int n) {

    // TODO
    long long result=1;
    if(n==0){
        result=1;
    }
    else{
        for(int i=1;i<=n;i++){
            result*=(long long)i;
        }
    }
    return result;
}

// ------------------- 测试代码，不要改 -------------------
static int g_pass = 0, g_fail = 0;
void check(bool ok, const std::string& name) {
    if (ok) { ++g_pass; std::cout << "  [PASS] " << name << "\n"; }
    else    { ++g_fail; std::cout << "  [FAIL] " << name << "\n"; }
}

int main() {
    std::cout << "===== 1. isPrime =====\n";
    check(isPrime(0) == false, "isPrime(0)");
    check(isPrime(1) == false, "isPrime(1)");
    check(isPrime(2) == true,  "isPrime(2)");
    check(isPrime(9) == false, "isPrime(9)");
    check(isPrime(17) == true, "isPrime(17)");
    check(isPrime(7919) == true, "isPrime(7919)");
    check(isPrime(7917) == false, "isPrime(7917)");
    check(isPrime(INT_MAX) == true, "isPrime(INT_MAX)");

    std::cout << "===== 2. gcd =====\n";
    check(gcd(12, 18) == 6,  "gcd(12,18)");
    check(gcd(18, 12) == 6,  "gcd(18,12)");
    check(gcd(17, 5) == 1,   "gcd(17,5)");
    check(gcd(0, 5) == 5,    "gcd(0,5)");
    check(gcd(-12, 18) == 6, "gcd(-12,18)");
    check(gcd(1071, 462) == 21, "gcd(1071,462)");

    std::cout << "===== 3. reverseString =====\n";
    { std::string s = "hello"; reverseString(s); check(s == "olleh", "reverse(hello)"); }
    { std::string s = "ab";    reverseString(s); check(s == "ba",    "reverse(ab)"); }
    { std::string s = "a";     reverseString(s); check(s == "a",     "reverse(a)"); }
    { std::string s = "";      reverseString(s); check(s == "",      "reverse(空串)"); }

    std::cout << "===== 4. sumArray =====\n";
    { int a[] = {1,2,3,4,5}; check(sumArray(a, 5) == 15, "sum{1..5}"); }
    check(sumArray(nullptr, 0) == 0, "空数�?");
    { int a[] = {-3,7,-1,0}; check(sumArray(a, 4) == 3, "sum{-3,7,-1,0}"); }
    { int a[] = {2000000000, 2000000000}; check(sumArray(a, 2) == 4000000000LL, "溢出测试"); }

    std::cout << "===== 5. factorial =====\n";
    check(factorial(0) == 1ULL, "0!");
    check(factorial(1) == 1ULL, "1!");
    check(factorial(5) == 120ULL, "5!");
    check(factorial(10) == 3628800ULL, "10!");
    check(factorial(20) == 2432902008176640000ULL, "20!");

    std::cout << "\n===== 结果�?" << g_pass << " 通过, " << g_fail << " 失败 =====\n";
    return g_fail == 0 ? 0 : 1;
}
