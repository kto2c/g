#include <sstream>

#include "gtest/gtest.h"
#include "word_automata.h"

TEST(word_bad, simple_mismatch) {
  std::stringstream s("a");
  EXPECT_EQ(0, tg::CountWords(s, "b"));
}

TEST(word_bad, empty_in) {
  std::stringstream s("");
  EXPECT_EQ(0, tg::CountWords(s, "b"));
}

TEST(word_bad, empty_word) {
  std::stringstream s("df");
  EXPECT_EQ(0, tg::CountWords(s, ""));
}

//////////////////////////////////////////

TEST(word_good, simple_match) {
  std::stringstream s("abracodobro");
  EXPECT_EQ(1, tg::CountWords(s, "abracodobro"));
}

TEST(word_good, trailing_quotes_match) {
  std::stringstream s("\"abracodobro\"");
  EXPECT_EQ(1, tg::CountWords(s, "abracodobro"));
}

TEST(word_good, trailing_space_match) {
  std::stringstream s(" \nabracodobro \t");
  EXPECT_EQ(1, tg::CountWords(s, "abracodobro"));
}

TEST(word_good, several_words_match) {
  std::stringstream s("\ta a\na aa aa\n\ra   ");
  EXPECT_EQ(4, tg::CountWords(s, "a"));
}

TEST(word_good, several_words_hard) {
  std::stringstream s("\t'mother' \t\rmotherr\n\n\r mother1 1mother\tmothe");
  EXPECT_EQ(1, tg::CountWords(s, "mother"));
}

TEST(word_chunk, chunk_boundary) {
  std::stringstream s("mother mother");
  EXPECT_EQ(2, tg::CountWords(s, "mother", 7));
}

TEST(word_chunk, word_longer_than_buffer) {
  std::stringstream s("1:abcdefghijklmnopqrstuvqxyz abcnotmatched "
	  "2:abcdefghijklmnopqrstuvqxyz");
  EXPECT_EQ(2, tg::CountWords(s, "abcdefghijklmnopqrstuvqxyz", 7));
}
