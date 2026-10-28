第二周DAY03
主要任务：先补45分钟std::string高频操作（size/substr/find/+=/c_str）；再做结构体+结构体指针 + ->；定义 DetResult{left,top,right,bottom,depth} 并写测试
预计练习：day03_结构体_盲写练习.cpp — 6题/26个断言

1.struct结构体需要先初始化才能赋值：例如对于struct DetResult {int cls_id; float prop; int left, top, right, bottom; float depth;};有初始化方法：DetResult a{1,0.9f,3,4,5,6,7.2f}（必须按顺序赋值，这种方法是初始化并赋值）、DetResult b{1,0.9f}（按顺序赋值部分成员后，剩余没赋值的成员自动补0，这种方法也是初始化并赋值）、DetResult c{};c.cls_id=1;c.left=2;（先将所有成员初始化为0，再对其中两个成员赋值，没被赋值的成员保持为0）。但如果写成DetResult d;d.left=1;d.depth=3.2f;就会报错，因为d没有初始化，里面的成员存的都是垃圾值，就算给left和depth赋值了，但其它成员还是随机垃圾值，所以这是未定义行为，会报错。

2.offsetof()做的事就一句话：某个成员，距离结构体开头有几个字节。​例如：offsetof(DetResult, depth)=24，意思就是“depth 这个成员，从 DetResult 开头往后数24个字节才开始”。offsetof 这个宏定义在头文件<cstddef>里，offsetof 在编译的时候就算完了，不花运行时间。

3.(->)可通过指针修改本体，例如struct DetResult {int cls_id; float prop; int left, top, right, bottom; float depth;};DetResult b{1,2.2f,3,4,5,6,9.3f};DetResult* p=&b;p->right=100;那么*p可以直接修改到b，此时b={1,2.2f,3,4,100,6,9.3f};如果想通过指针访问b，那么需要先解引再用(.)，即cout<<(*p).left;就会输出3。

4.sizeof()算结构体占了多少字节的时候有内存对齐的原则，编译器会按最大成员宽度补齐，让每个成员都落在对齐边界上。例如sizeof(struct{int a;char c;})=8（成员最大宽度为int=4，所以char会被补齐到4）、sizeof(struct{int a;double c;})=16（成员最大宽度是8，int会被补齐到8字节）。
