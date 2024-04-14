#pragma once
#include<istream>
#include <string_view>

namespace tg {
// word allows only symbols A-Za-z0-9_. No validation for it
size_t countWords(std::istream& in, const std::string_view& word, size_t buf_size_bytes = 4096);
}  // namespace tg
