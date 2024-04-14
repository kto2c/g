#include <iostream>

#include "argparse/args.h"

int main(int argc, const char* const argv[]) {
  std::cout << "Hello World" << std::endl;

  tg::ArgParser argparser;
  argparser.registerArg("h", false);
  argparser.registerArg("f", true);
  argparser.registerArg("m", true);
  argparser.registerArg("v", true);

  auto ans = argparser.parseArgs(argc, argv);
  try {
    auto res = std::get<std::string>(ans);
    std::cout << res;
  } catch (std::bad_variant_access) {
  };
  try {
    auto res = std::get<tg::ArgParser::Args>(ans);
    for (auto&& [k, v] : res) {
      std::cout << k << " " << v.value_or("") << std::endl;
    }
  } catch (std::bad_variant_access) {
  }
}