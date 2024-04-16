#pragma once
#include <cstdint>
#include <istream>

namespace tg {
uint32_t Sum32(std::istream& in, size_t buf_size_bytes = 4096);
}  // namespace tg
