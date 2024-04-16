#include <sstream>

#include "gtest/gtest.h"
#include "sum32.h"

TEST(sum32, simple1) {
  std::stringstream s("1");
  EXPECT_EQ(0x31000000, tg::Sum32(s));
}

TEST(sum32, simple12) {
  std::stringstream s("12");
  EXPECT_EQ(0x31320000, tg::Sum32(s));
}

TEST(sum32, simple123) {
  std::stringstream s("123");
  EXPECT_EQ(0x31323300, tg::Sum32(s));
}

TEST(sum32, simple1234) {
  std::stringstream s("1234");
  EXPECT_EQ(0x31323334, tg::Sum32(s));
}

TEST(sum32, small_buf) {
  std::stringstream s("12345");
  EXPECT_EQ(0x66323334, tg::Sum32(s, 3));
}

TEST(sum32, zero) {
  std::stringstream s("\xfe\xfe\xfe\xff\x1\x1\x1\x1\0");
  EXPECT_EQ(0, tg::Sum32(s));
}
