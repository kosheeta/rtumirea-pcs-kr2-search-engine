#include <gtest/gtest.h>

#include "core/Tokenizer.h"

TEST(TokenizerTest, EmptyStringReturnsEmptyTokens) {
    Tokenizer tokenizer;
    EXPECT_TRUE(tokenizer.tokenize("").empty());
}

TEST(TokenizerTest, WhitespaceOnlyReturnsEmptyTokens) {
    Tokenizer tokenizer;
    EXPECT_TRUE(tokenizer.tokenize("  \n\t  \r\n").empty());
}

TEST(TokenizerTest, MixedCaseIsConvertedToLowerCase) {
    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize("Search Engine");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "search");
    EXPECT_EQ(tokens[1], "engine");
}

TEST(TokenizerTest, PunctuationSplitsTokensCorrectly) {
    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize("hello, world!!!");

    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0], "hello");
    EXPECT_EQ(tokens[1], "world");
}

TEST(TokenizerTest, NumbersArePreservedInTokens) {
    Tokenizer tokenizer;
    const auto tokens = tokenizer.tokenize("v2 model 2026");

    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0], "v2");
    EXPECT_EQ(tokens[1], "model");
    EXPECT_EQ(tokens[2], "2026");
}
