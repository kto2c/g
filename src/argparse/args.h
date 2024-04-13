#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace tg {
class ArgParser final {
  // arg name->has value (-h has no values, -f has value)
  std::unordered_map<std::string, bool> registered_args;

 public:
  using Args = std::unordered_map<std::string, std::optional<std::string_view>>;
  using Err = std::string;
  using ArgsExpected = std::variant<Args, Err>;  // C++23 std::expected

  void registerArg(const std::string& name, bool has_value);
  ArgsExpected parseArgs(int argc, const char *const argv[]) const;
};
}  // namespace tg
