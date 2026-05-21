#include <algorithm>

#include <gtest/gtest.h>

#include "core/InvertedIndex.h"

TEST(InvertedIndexTest, AddNewTokenInNewDocument) {
    InvertedIndex index;
    index.addToken("search", "doc1.txt");

    const auto postings = index.search("search");
    ASSERT_EQ(postings.size(), 1);
    EXPECT_EQ(postings[0].documentPath, "doc1.txt");
    EXPECT_EQ(postings[0].frequency, 1);
}

TEST(InvertedIndexTest, RepeatedTokenInSameDocumentIncrementsFrequency) {
    InvertedIndex index;
    index.addToken("search", "doc1.txt");
    index.addToken("search", "doc1.txt");
    index.addToken("search", "doc1.txt");

    const auto postings = index.search("search");
    ASSERT_EQ(postings.size(), 1);
    EXPECT_EQ(postings[0].frequency, 3);
}

TEST(InvertedIndexTest, SameTokenInDifferentDocumentsCreatesMultiplePostings) {
    InvertedIndex index;
    index.addToken("search", "doc1.txt");
    index.addToken("search", "doc2.txt");

    const auto postings = index.search("search");
    ASSERT_EQ(postings.size(), 2);
}

TEST(InvertedIndexTest, MissingTokenReturnsEmptyResult) {
    InvertedIndex index;
    index.addToken("search", "doc1.txt");

    EXPECT_TRUE(index.search("engine").empty());
}

TEST(InvertedIndexTest, SearchIsEmptyAfterClear) {
    InvertedIndex index;
    index.addToken("search", "doc1.txt");
    index.clear();

    EXPECT_TRUE(index.search("search").empty());
}

TEST(InvertedIndexTest, ClearResetsUniqueTokenCount) {
    InvertedIndex index;
    index.addToken("a", "doc1");
    index.addToken("b", "doc1");
    index.clear();

    EXPECT_EQ(index.getUniqueTokenCount(), 0);
}

TEST(InvertedIndexTest, ClearResetsDocumentCount) {
    InvertedIndex index;
    index.addToken("a", "doc1");
    index.addToken("b", "doc2");
    index.clear();

    EXPECT_EQ(index.getDocumentCount(), 0);
}

TEST(InvertedIndexTest, SupportsRefillAfterClear) {
    InvertedIndex index;
    index.addToken("a", "doc1");
    index.clear();
    index.addToken("b", "doc2");

    const auto postings = index.search("b");
    ASSERT_EQ(postings.size(), 1);
    EXPECT_EQ(postings[0].documentPath, "doc2");
}

TEST(InvertedIndexTest, DocumentCountTracksUniqueDocuments) {
    InvertedIndex index;
    index.addToken("a", "doc1");
    index.addToken("a", "doc1");
    index.addToken("b", "doc2");

    EXPECT_EQ(index.getDocumentCount(), 2);
}

TEST(InvertedIndexTest, UniqueTokenCountTracksUniqueTokens) {
    InvertedIndex index;
    index.addToken("a", "doc1");
    index.addToken("a", "doc2");
    index.addToken("b", "doc1");

    EXPECT_EQ(index.getUniqueTokenCount(), 2);
}
