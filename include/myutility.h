#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace ljh::utility {
std::vector<std::string> SplitString(const std::string& str, char delimiter);
}