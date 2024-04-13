#include "args.h"

#include <format>

namespace tg {
void ArgParser::registerArg(const std::string& name, bool has_value) {
  registered_args[name] = has_value;
}

ArgParser::ArgsExpected ArgParser::parseArgs(int argc,
                                             const char *const argv[]) const {
  Args ans;
  for (int i = 1; i < argc; ++i) {
    const auto cur_arg = argv[i];
    const auto cur_arg_len = strlen(cur_arg);
    if (cur_arg_len < 2) {
      return std::format("Bad {}st arg '{}'", i, cur_arg);
    }

    if (cur_arg[0] != '-') {
      return std::format("Flag without minus '{}' is forbidden", cur_arg);
    }

    const std::string clear_arg = &cur_arg[1];

    bool has_value{};
    {
      auto it = registered_args.find(clear_arg);
      if (it == registered_args.end()) {
        return std::format("Unrecognized flag '{}'", clear_arg);
      }
      has_value = it->second;
    }

    auto [it, ok] = ans.try_emplace(clear_arg);
    if (!ok) {
      return std::format("Arg '{}' duplicated", it->first);
    }

    if (!has_value) {
      continue;
    }

    ++i;
    if (i >= argc) {
      return std::format("Arg '{}' must have value", it->first);
    }
    it->second = argv[i];
  }
  return ans;
}
}  // namespace tg