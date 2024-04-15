#include "sum32.h"

#include <cassert>
#include <vector>

namespace tg {

uint32_t Sum32(std::istream& in, size_t buf_size_bytes) {
  assert(buf_size_bytes > 0);

  uint32_t ans = 0;
  std::vector<char> buf(buf_size_bytes);
  int offset = 24;
  while (in.good()) {
    in.read(buf.data(), buf_size_bytes);
    buf.resize(in.gcount());

    for (const char c : buf) {
      ans += static_cast<uint32_t>(static_cast<uint8_t>(c)) << offset;
      offset = offset > 0 ? offset - 8 : 24;
    }
  }
  return ans;
}
}  // namespace tg