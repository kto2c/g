#include "word_automata.h"

#include <cassert>
#include <vector>

namespace tg {
namespace {
enum class State { noMatch, wordStartMatched, subwordMatched };
}

size_t CountWords(std::istream& in, const std::string_view& word,
                  const size_t buf_size_bytes) {
  assert(buf_size_bytes > 0);

  size_t ans = 0;
  if (word.empty()) {
    return ans;
  }

  const auto is_word_boundary = [](char c) {
    return c < '0' || (c > '9' && c < 'A') || (c > 'Z' && c < '_') ||
           c == '`' || c > 'z';
  };

  State cur_state = State::wordStartMatched;
  size_t matched_syms_num{};
  std::vector<char> buf(buf_size_bytes);
  while (in.good()) {
    in.read(buf.data(), buf_size_bytes);
    buf.resize(in.gcount());

    for (const char c : buf) {
      switch (cur_state) {
        case State::noMatch:
          if (is_word_boundary(c)) {
            cur_state = State::wordStartMatched;
            matched_syms_num = 0;
          }
          break;
        case State::wordStartMatched:
          matched_syms_num = 0;
          if (c == word[matched_syms_num]) {
            cur_state = State::subwordMatched;
            ++matched_syms_num;
            continue;
          }
          if (is_word_boundary(c)) {
            continue;
          }
          cur_state = State::noMatch;
          matched_syms_num = 0;
          break;
        case State::subwordMatched:
          if (is_word_boundary(c)) {
            if (matched_syms_num == word.size()) {
              ++ans;
            }
            cur_state = State::wordStartMatched;
            matched_syms_num = 0;
            continue;
          }

          if (matched_syms_num >= word.size()) {
            cur_state = State::noMatch;
            matched_syms_num = 0;
            continue;
          }
          if (c == word[matched_syms_num]) {
            ++matched_syms_num;
            continue;
          }
          cur_state = State::noMatch;
          matched_syms_num = 0;
      }
    }
  }
  if (matched_syms_num == word.size()) {
    ++ans;
  }
  return ans;
}
}  // namespace tg