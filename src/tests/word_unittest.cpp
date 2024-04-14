#include "word_automata.h"
#include "gtest/gtest.h"
#include <sstream>

TEST(word_bad, simple_mismatch) {
  std::stringstream s("a");
  EXPECT_EQ(0, tg::countWords(s, "b"));
}

TEST(word_bad, empty_in) {
  std::stringstream s("");
  EXPECT_EQ(0, tg::countWords(s, "b"));
}

TEST(word_bad, empty_word) {
  std::stringstream s("df");
  EXPECT_EQ(0, tg::countWords(s, ""));
}

//////////////////////////////////////////

TEST(word_good, simple_match) {
  std::stringstream s("abracodobro");
  EXPECT_EQ(1, tg::countWords(s, "abracodobro"));
}

TEST(word_good, trailing_quotes_match) {
  std::stringstream s("\"abracodobro\"");
  EXPECT_EQ(1, tg::countWords(s, "abracodobro"));
}

TEST(word_good, trailing_space_match) {
  std::stringstream s(" \nabracodobro \t");
  EXPECT_EQ(1, tg::countWords(s, "abracodobro"));
}

TEST(word_good, several_words_match) {
    std::stringstream s("\ta a\na aa aa\n\ra   ");
    EXPECT_EQ(4, tg::countWords(s, "a"));
  }

TEST(word_good, several_words_hard) {
    std::stringstream s("\t'mother' \t\rmotherr\n\n\r mother1 1mother\tmothe");
    EXPECT_EQ(1, tg::countWords(s, "mother"));
  }

TEST(word_chunk, chunk_boundary) {
    std::stringstream s("mother mother");
    EXPECT_EQ(2, tg::countWords(s, "mother", 7));
  }
