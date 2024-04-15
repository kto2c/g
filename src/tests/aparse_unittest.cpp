#include "args.h"
#include "gtest/gtest.h"

#ifdef _WIN32
// disable warning of ignored value of nodiscard function get
#pragma warning(disable : 4834)
#pragma warning(disable : 6031)
#endif

TEST(args_bad, only_minus) {
  using Parser = tg::ArgParser;
  Parser parser;
  const char* args[] = {"unittest", "-"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, only_double_minus) {
  using Parser = tg::ArgParser;
  Parser parser;
  const char* args[] = {"unittest", "--"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, only_minus_twice) {
  using Parser = tg::ArgParser;
  Parser parser;
  const char* args[] = {"unittest", "-", "-"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, unregistered_minus) {
  using Parser = tg::ArgParser;
  Parser parser;
  const char* args[] = {"unittest", "-a"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, unregistered_double_minus) {
  using Parser = tg::ArgParser;
  Parser parser;
  const char* args[] = {"unittest", "--a"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, unregistered_val) {
  using Parser = tg::ArgParser;
  Parser parser;
  const char* args[] = {"unittest", "-a", "a"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, double_help) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  const char* args[] = {"unittest", "-h", "-h"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, help) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  const char* args[] = {"unittest", "h"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, second_arg_isnt_a_flag) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  const char* args[] = {"unittest", "-h", "h"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, arg_expects_val) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  parser.RegisterArg("f", true);
  const char* args[] = {"unittest", "-h", "-f"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_bad, pseudo_help) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  const char* args[] = {"unittest", "-h1"};
  const auto size = static_cast<int>(std::size(args));
  EXPECT_NO_THROW(std::get<Parser::Err>(parser.ParseArgs(size, args)));
}

TEST(args_good, help) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  parser.RegisterArg("f", true);
  parser.RegisterArg("m", true);
  parser.RegisterArg("v", true);
  const char* args[] = {"unittest", "-h"};
  const auto size = static_cast<int>(std::size(args));
  Parser::Args expected = {{"h", std::optional<std::string_view>{}}};
  EXPECT_EQ(std::get<Parser::Args>(parser.ParseArgs(size, args)), expected);
}

TEST(args_good, file) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  parser.RegisterArg("f", true);
  parser.RegisterArg("m", true);
  parser.RegisterArg("v", true);
  const char* args[] = {"unittest", "-f", "test"};
  const auto size = static_cast<int>(std::size(args));
  Parser::Args expected = {{"f", "test"}};
  EXPECT_EQ(std::get<Parser::Args>(parser.ParseArgs(size, args)), expected);
}

TEST(args_good, mode) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  parser.RegisterArg("f", true);
  parser.RegisterArg("m", true);
  parser.RegisterArg("v", true);
  const char* args[] = {"unittest", "-m", "checksum"};
  const auto size = static_cast<int>(std::size(args));
  Parser::Args expected = {{"m", "checksum"}};
  EXPECT_EQ(std::get<Parser::Args>(parser.ParseArgs(size, args)), expected);
}

TEST(args_good, pattern) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  parser.RegisterArg("f", true);
  parser.RegisterArg("m", true);
  parser.RegisterArg("v", true);
  const char* args[] = {"unittest", "-v", "mother"};
  const auto size = static_cast<int>(std::size(args));
  Parser::Args expected = {{"v", "mother"}};
  EXPECT_EQ(std::get<Parser::Args>(parser.ParseArgs(size, args)), expected);
}

TEST(args_good, all) {
  using Parser = tg::ArgParser;
  Parser parser;
  parser.RegisterArg("h", false);
  parser.RegisterArg("f", true);
  parser.RegisterArg("m", true);
  parser.RegisterArg("v", true);
  const char* args[] = {"unittest", "-f", "Test.tst", "-m",
                        "words",    "-v", "mother"};
  const auto size = static_cast<int>(std::size(args));
  Parser::Args expected = {{"f", "Test.tst"}, {"m", "words"}, {"v", "mother"}};
  EXPECT_EQ(std::get<Parser::Args>(parser.ParseArgs(size, args)), expected);
}
