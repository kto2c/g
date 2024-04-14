#include <cassert>
#include <vector>
#include "word_automata.h"

namespace tg {
namespace {
enum class State { noMatch, wordStartMatched, subwordMatched };
}


size_t countWords(std::istream& in, const std::string_view& word,
                  size_t buf_size_bytes) {
  assert(buf_size_bytes > 0);

  size_t ans = 0;
  if (word.empty()) {
    return ans;
  }

  const auto isWordBoundary = [](char c) {
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
        // noMatch todo case "mothers"
        case State::noMatch:
          if (isWordBoundary(c)) {
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
          if (isWordBoundary(c)) {
            continue;
          }
          cur_state = State::noMatch;
          matched_syms_num = 0;
          break;
        case State::subwordMatched:
          if (isWordBoundary(c)) {
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