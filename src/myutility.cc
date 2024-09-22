#include "myutility.h"

namespace ljh::utility {
// 递归函数，用于分割字符串
std::vector<std::string> SplitString(const std::string& str, char delimiter) {
    // 基本情况：如果字符串为空，返回空 vector
    if (str.empty()) {
        return {};
    }

    // 查找分隔符的位置
    size_t pos = str.find(delimiter);

    // 如果没有找到分隔符，则返回当前字符串
    if (pos == std::string::npos) {
        return {str};
    }

    // 获取当前分隔符前的部分
    std::string token = str.substr(0, pos);

    // 递归处理剩余部分
    std::vector<std::string> result = SplitString(str.substr(pos + 1), delimiter);

    // 将当前部分加入结果
    result.insert(result.begin(), token);

    return result;
}
}  // namespace ljh::utility