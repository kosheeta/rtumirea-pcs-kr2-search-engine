#include <gtest/gtest.h>

#include "core/InvertedIndex.h"
#include "core/Searcher.h"
#include "core/Tokenizer.h"

namespace {

Searcher makeSearcher(Tokenizer& tokenizer, InvertedIndex& index) {
    return Searcher(&tokenizer, &index);
}

}

TEST(SearcherTest, ExactSingleTokenSearchReturnsExpectedDocuments) {
    Tokenizer tokenizer;
    InvertedIndex index;
    index.addToken("search", "doc1.txt");
    index.addToken("search", "doc1.txt");
    index.addToken("search", "doc2.txt");

    auto searcher = makeSearcher(tokenizer, index);
    const auto results = searcher.search("search");

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].matchType, MatchType::EXACT);
    EXPECT_GE(results[0].frequency, results[1].frequency);
}

TEST(SearcherTest, EmptyQueryReturnsNoResults) {
    Tokenizer tokenizer;
    InvertedIndex index;
    auto searcher = makeSearcher(tokenizer, index);

    EXPECT_TRUE(searcher.search("   \n\t").empty());
}

TEST(SearcherTest, MultiTokenQueryMergesDocumentsAcrossTokens) {
    Tokenizer tokenizer;
    InvertedIndex index;
    index.addToken("alpha", "doc1.txt");
    index.addToken("beta", "doc2.txt");

    auto searcher = makeSearcher(tokenizer, index);
    const auto results = searcher.search("alpha beta");

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].matchType, MatchType::EXACT);
    EXPECT_EQ(results[1].matchType, MatchType::EXACT);
}

TEST(SearcherTest, FallsBackToFuzzyWhenNoExactMatches) {
    Tokenizer tokenizer;
    InvertedIndex index;
    index.addToken("search", "doc1.txt");

    auto searcher = makeSearcher(tokenizer, index);
    const auto results = searcher.search("serch");

    ASSERT_FALSE(results.empty());
    EXPECT_EQ(results[0].matchType, MatchType::FUZZY);
    EXPECT_EQ(results[0].matchedToken, "search");
}

TEST(SearcherTest, FuzzyThresholdIncludesDistanceTwoAndExcludesGreater) {
    Tokenizer tokenizer;
    InvertedIndex index;
    index.addToken("book", "doc1.txt");
    index.addToken("binary", "doc2.txt");

    auto searcher = makeSearcher(tokenizer, index);
    const auto nearResults = searcher.search("boon");
    ASSERT_FALSE(nearResults.empty());
    EXPECT_EQ(nearResults[0].matchedToken, "book");

    const auto farResults = searcher.search("zzzzzz");
    EXPECT_TRUE(farResults.empty());
}

TEST(SearcherTest, ExactResultsAreSortedByFrequencyDescending) {
    Tokenizer tokenizer;
    InvertedIndex index;
    index.addToken("engine", "doc1.txt");
    index.addToken("engine", "doc1.txt");
    index.addToken("engine", "doc2.txt");

    auto searcher = makeSearcher(tokenizer, index);
    const auto results = searcher.search("engine");

    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].matchType, MatchType::EXACT);
    EXPECT_GT(results[0].frequency, results[1].frequency);
}

TEST(SearcherTest, FuzzyResultsSortedByDistanceThenFrequency) {
    Tokenizer tokenizer;
    InvertedIndex index;
    index.addToken("cart", "doc1.txt");
    index.addToken("cart", "doc1.txt");
    index.addToken("cat", "doc2.txt");

    auto searcher = makeSearcher(tokenizer, index);
    const auto results = searcher.search("car");

    ASSERT_FALSE(results.empty());
    ASSERT_EQ(results[0].matchType, MatchType::FUZZY);
    EXPECT_EQ(results[0].matchedToken, "cart");
}

TEST(SearcherTest, ExactResultsComeBeforeFuzzyResults) {
    Tokenizer tokenizer;
    InvertedIndex index;
    index.addToken("alpha", "exact_doc.txt");
    index.addToken("bravo", "fuzzy_doc.txt");

    auto searcher = makeSearcher(tokenizer, index);
    const auto results = searcher.search("alpha brvo");

    ASSERT_GE(results.size(), 2);
    EXPECT_EQ(results[0].matchType, MatchType::EXACT);
    EXPECT_EQ(results[1].matchType, MatchType::FUZZY);
}

TEST(SearcherTest, ResultContainsAllRequiredFields) {
    Tokenizer tokenizer;
    InvertedIndex index;
    index.addToken("search", "doc1.txt");

    auto searcher = makeSearcher(tokenizer, index);
    const auto results = searcher.search("search");

    ASSERT_EQ(results.size(), 1);
    EXPECT_EQ(results[0].path, "doc1.txt");
    EXPECT_EQ(results[0].matchedToken, "search");
    EXPECT_GT(results[0].frequency, 0);
    EXPECT_EQ(results[0].distance, 0);
}
