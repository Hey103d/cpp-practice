// day02b_字符串补课.cpp
// 45 分钟补课包：v2 计划里被挤掉的"数组与字符串"，只补毕设真正用得上的部分
// 编译运行：g++ -std=c++17 -Wall -Wextra -Wshadow -O2 day02b_字符串补课.cpp -o s && ./s
#include <iostream>
#include <string>


// ============================================================
// 第一部分：std::string 的 8 个高频操作（先跑一遍看输出）
// 判断标准只有一条：rknn_model_zoo 里会不会出现？
// ============================================================
void demoString() {
	std::cout << "===== std::string 八个高频操作 =====\n";

	std::string name = "yolov8.rknn";

	// 1. 长度：length() 和 size() 完全等价，size() 更通用（vector 也有）
	std::cout << "1. name.size() = " << name.size()
	          << "，name.length() = " << name.length() << "\n";

	// 2. 拼接：+= 比 + 快（不产生临时对象），循环里一定用 +=
	std::string path = "/data/models/";
	path += name;
	std::cout << "2. 拼接后 path = " << path << "\n";

	// 3. find：找不到返回 std::string::npos（不是 -1，也不是 0）
	size_t dot = name.find('.');
	std::cout << "3. find('.') = " << dot
	          << "，没找到时返回 npos = " << std::string::npos << "\n";
	if (name.find(".onnx") == std::string::npos) {
		std::cout << "   name 里没有 .onnx（这就是标准写法，别写成 == -1）\n";
	}

	// 4. substr(起点, 长度)：第二个参数是【长度】，不是结束下标
	std::cout << "4. 主文件名 = " << name.substr(0, dot)
	          << "，扩展名 = " << name.substr(dot) << "\n";

	// 5. c_str()：把 std::string 变成 const char*，传给 C 风格 API
	//    毕设里 rknn_init 要 const char* 模型路径，就要用它
	const char* cpath = path.c_str();
	std::cout << "5. c_str() = " << cpath << "\n";

	// 6. 反向：const char* → std::string 直接构造
	std::string back(cpath);
	std::cout << "6. 转回来 back == path ? " << (back == path ? "是" : "否") << "\n";

	// 7. 遍历字符：范围 for 最省事
	int dots = 0;
	for (char ch : name) if (ch == '.') ++dots;
	std::cout << "7. 范围 for 数点号 = " << dots << " 个\n";

	// 8. 比较：直接用 ==，不要用 strcmp
	std::cout << "8. name == \"yolov8.rknn\" ? " << (name == "yolov8.rknn" ? "是" : "否") << "\n\n";
}

// ============================================================
// 第二部分：三道题，都是毕设里真的会写的函数
// ============================================================

// 题 1：取扩展名（含点）。没有扩展名返回空串。
// 例："/mnt/yolov8.rknn" -> ".rknn"；"yolov8" -> ""
// 提示：用 find_last_of(".") 找最后一个点（路径里可能有多个点，比如 ./a.b.rknn）
std::string getExt(const std::string& path) {
	// TODO
	if(path=="\0")return{"\0"};
	int b=path.find_last_of('.');
	if(b==std::string::npos)return {"\0"};
	std::string str=path.substr(b,path.size());
	return str;
}

// 题 2：取文件名（去掉目录）。支持 / 和 \ 两种分隔符。
// 例："/mnt/models/yolov8.rknn" -> "yolov8.rknn"；"a\\b.txt" -> "b.txt"
// 提示：find_last_of("/\\")，找不到就返回原串
std::string getFileName(const std::string& path) {
	// TODO
	size_t b=path.find_last_of("\\/");
	if(b==std::string::npos)return path;
	return path.substr(b+1,path.size());
}

// 题 3：后缀判断。判断 s 是不是以 suf 结尾。
// 例：endsWith("yolov8.rknn", ".rknn") -> true
// 提示：如果 suf 比 s 还长，直接返回 false，否则用 compare 或 substr
bool endsWith(const std::string& s, const std::string& suf) {
	// TODO
	if(suf.size()>s.size())return false;
	std::string str=s.substr(s.size()-suf.size(),s.size());
	if(str==suf)return true;
	else return false;
}

// ------------------------- 测试代码，不要改 -------------------------
static int g_pass = 0, g_fail = 0;
void check(bool ok, const std::string& n) {
	if (ok) { ++g_pass; std::cout << "  [PASS] " << n << "\n"; }
	else    { ++g_fail; std::cout << "  [FAIL] " << n << "\n"; }
}

int main() {
	std::cout << std::unitbuf;
	demoString();

	std::cout << "===== 题 1. getExt =====\n";
	check(getExt("/mnt/models/yolov8.rknn") == ".rknn", "普通路径");
	check(getExt("yolov8.onnx") == ".onnx", "相对路径");
	check(getExt("yolov8") == "", "没有扩展名");
	check(getExt("./cfg/a.b.rknn") == ".rknn", "路径里有多个点");
	check(getExt("") == "", "空串不崩");

	std::cout << "===== 题 2. getFileName =====\n";
	check(getFileName("/mnt/models/yolov8.rknn") == "yolov8.rknn", "Linux 路径");
	check(getFileName("a\\b.txt") == "b.txt", "Windows 路径");
	check(getFileName("yolov8.rknn") == "yolov8.rknn", "没有目录");
	check(getFileName("") == "", "空串不崩");

	std::cout << "===== 题 3. endsWith =====\n";
	check(endsWith("yolov8.rknn", ".rknn") == true, "标准情况");
	check(endsWith("yolov8.rknn", ".onnx") == false, "不匹配");
	check(endsWith("a", "abc") == false, "后缀比原串长（最容易越界）");
	check(endsWith("abc", "abc") == true, "完全相同");
	check(endsWith("abc", "") == true, "空后缀算真");

	std::cout << "\n===== 结果：" << g_pass << " 通过, " << g_fail << " 失败 =====\n";
	return g_fail == 0 ? 0 : 1;
}
