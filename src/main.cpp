#include <cassert>
#include <fstream>
#include <iostream>
#include <ranges>

#include "algo/sum32.h"
#include "algo/word_automata.h"
#include "argparse/args.h"

constexpr const char* kFileFlag = "f";
constexpr const char* kModeFlag = "m";
constexpr const char* kWordFlag = "v";
constexpr const char* kHelpFlag = "h";

static void PrintHelp() {
  std::cout
      << "Usage:\n"
         "1. test -f Test.tst -m words -v mother -- prints count of word "
         "'mother' from 'Test.tst' file (case sensitive)\n"
         "2. test -f Test.tst -m checksum -- prints 32-bit checksum, where "
         "checksum = word1 + word2 + ... + wordN (word1..wordN -- 32-bit "
         "words from the file)\n"
         "3. test -h -- prints this message."
      << std::endl;
}

struct ProcessArgs final {
  int operator()(tg::ArgParser::Args args) {
    if (args.find(kHelpFlag) != args.end()) {
      if (args.size() > 1) {
        std::cerr << "Unexpected args";
        for (auto arg : args | std::views::keys |
                            std::views::filter([](const std::string& s) {
                              return s != kHelpFlag;
                            })) {
          std::cerr << " -" << arg << std::endl;
        }
        PrintHelp();
        return 1;
      }
      PrintHelp();
      return 0;
    }
    auto file_it = args.find(kFileFlag);
    auto mode_it = args.find(kModeFlag);
    if (file_it == args.end()) {
      std::cerr << "Expected '-f' arg" << std::endl;
      return 1;
    }
    if (mode_it == args.end()) {
      std::cerr << "Expected '-m' arg" << std::endl;
      return 1;
    }
    assert(file_it->second.has_value() && mode_it->second.has_value());

    auto file_path = file_it->second.value();
    auto mode = mode_it->second.value();
    std::ifstream in(file_it->second.value().data(), mode == "checksum"
                                                         ? std::ios_base::binary
                                                         : std::ios_base::in);
    if (!in.good()) {
      std::cerr << "Can't read file " << file_path << std::endl;
      return 2;
    }

    if (mode == "checksum") {
      std::cout << tg::Sum32(in) << std::endl;
      return 0;
    }
    if (mode != "words") {
      std::cerr << "Unexpected mode '" << mode << "'" << std::endl;
      return 1;
    }
    auto word_it = args.find(kWordFlag);
    if (word_it == args.end()) {
      std::cerr << "Expected '-v' arg" << std::endl;
      return 1;
    }
    assert(word_it->second.has_value());
    std::cout << tg::CountWords(in, word_it->second.value()) << std::endl;
    return 0;
  }

  int operator()(const tg::ArgParser::Err& err) {
    std::cerr << err << std::endl;
    return 1;
  }
};

int main(int argc, const char* const argv[]) {
  tg::ArgParser argparser;
  argparser.RegisterArg(kHelpFlag, false);
  argparser.RegisterArg(kFileFlag, true);
  argparser.RegisterArg(kModeFlag, true);
  argparser.RegisterArg(kWordFlag, true);

  auto args = argparser.ParseArgs(argc, argv);
  return std::visit(ProcessArgs(), args);
}
