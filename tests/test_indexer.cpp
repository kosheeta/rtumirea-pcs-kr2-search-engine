#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

#include "core/Indexer.h"

namespace {

class FakeFileReader : public IFileReader {
public:
    std::vector<std::string> files;
    std::unordered_map<std::string, std::string> fileContents;

    std::vector<std::string> listFiles(const std::string&) override {
        return files;
    }

    std::string readFile(const std::string& path) override {
        const auto it = fileContents.find(path);
        if (it == fileContents.end()) {
            return {};
        }
        return it->second;
    }
};

class FakeTokenizer : public ITokenizer {
public:
    std::vector<std::string> tokenize(const std::string& text) override {
        std::vector<std::string> out;
        std::string current;
        for (char c : text) {
            if (c == ' ') {
                if (!current.empty()) {
                    out.push_back(current);
                    current.clear();
                }
            } else {
                current.push_back(c);
            }
        }
        if (!current.empty()) {
            out.push_back(current);
        }
        return out;
    }
};

class FakeInvertedIndex : public IInvertedIndex {
public:
    bool clearCalled = false;
    std::vector<std::pair<std::string, std::string>> addCalls;
    std::vector<std::string> knownTokens;

    void clear() override {
        clearCalled = true;
        addCalls.clear();
    }

    void addToken(const std::string& token, const std::string& docPath) override {
        addCalls.emplace_back(token, docPath);
    }

    std::vector<Posting> search(const std::string&) const override {
        return {};
    }

    std::vector<std::string> getAllTokens() const override {
        return knownTokens;
    }

    std::size_t getDocumentCount() const override {
        std::unordered_set<std::string> docs;
        for (const auto& call : addCalls) {
            docs.insert(call.second);
        }
        return docs.size();
    }

    std::size_t getUniqueTokenCount() const override {
        std::unordered_set<std::string> tokens;
        for (const auto& call : addCalls) {
            tokens.insert(call.first);
        }
        return tokens.size();
    }
};

}

TEST(IndexerTest, HappyPathBuildsIndexAndReportsFinishedStats) {
    FakeFileReader fileReader;
    FakeTokenizer tokenizer;
    FakeInvertedIndex index;
    fileReader.files = {"doc1.txt", "doc2.txt"};
    fileReader.fileContents["doc1.txt"] = "search engine search";
    fileReader.fileContents["doc2.txt"] = "engine";

    Indexer indexer(&fileReader, &tokenizer, &index);
    std::vector<IndexingStatus> statuses;
    indexer.setStatusCallback([&statuses](const IndexingStatus& status) { statuses.push_back(status); });
    indexer.setSourcePath("/tmp/src");
    indexer.buildIndex();

    EXPECT_TRUE(index.clearCalled);
    EXPECT_EQ(index.getDocumentCount(), 2);
    EXPECT_EQ(index.getUniqueTokenCount(), 2);
    ASSERT_FALSE(statuses.empty());
    EXPECT_EQ(statuses.front().eventType, IndexingEventType::STARTED);
    EXPECT_EQ(statuses.back().eventType, IndexingEventType::FINISHED);
    EXPECT_EQ(statuses.back().processedFiles, 2);
}

TEST(IndexerTest, EmptyDirectoryProducesEmptyIndex) {
    FakeFileReader fileReader;
    FakeTokenizer tokenizer;
    FakeInvertedIndex index;

    Indexer indexer(&fileReader, &tokenizer, &index);
    indexer.setSourcePath("/tmp/empty");
    indexer.buildIndex();

    EXPECT_TRUE(index.clearCalled);
    EXPECT_EQ(index.getDocumentCount(), 0);
    EXPECT_EQ(index.getUniqueTokenCount(), 0);
}

TEST(IndexerTest, EmptyFileContentDoesNotBreakIndexingOtherFiles) {
    FakeFileReader fileReader;
    FakeTokenizer tokenizer;
    FakeInvertedIndex index;
    fileReader.files = {"good.txt", "bad.txt"};
    fileReader.fileContents["good.txt"] = "hello world";
    fileReader.fileContents["bad.txt"] = "";

    Indexer indexer(&fileReader, &tokenizer, &index);
    indexer.setSourcePath("/tmp/src");
    indexer.buildIndex();

    EXPECT_EQ(index.getDocumentCount(), 1);
    EXPECT_EQ(index.getUniqueTokenCount(), 2);
}

TEST(IndexerTest, ReindexingWithDifferentInputReplacesState) {
    FakeFileReader fileReader;
    FakeTokenizer tokenizer;
    FakeInvertedIndex index;
    fileReader.files = {"doc1.txt"};
    fileReader.fileContents["doc1.txt"] = "alpha";

    Indexer indexer(&fileReader, &tokenizer, &index);
    indexer.setSourcePath("/tmp/src1");
    indexer.buildIndex();
    EXPECT_EQ(index.getUniqueTokenCount(), 1);

    fileReader.files = {"doc2.txt"};
    fileReader.fileContents.clear();
    fileReader.fileContents["doc2.txt"] = "beta gamma";
    indexer.setSourcePath("/tmp/src2");
    indexer.buildIndex();

    EXPECT_EQ(index.getDocumentCount(), 1);
    EXPECT_EQ(index.getUniqueTokenCount(), 2);
}

TEST(IndexerTest, EmptySourcePathEmitsWarningAndSkipsProcessing) {
    FakeFileReader fileReader;
    FakeTokenizer tokenizer;
    FakeInvertedIndex index;

    Indexer indexer(&fileReader, &tokenizer, &index);
    std::vector<IndexingStatus> statuses;
    indexer.setStatusCallback([&statuses](const IndexingStatus& status) { statuses.push_back(status); });
    indexer.buildIndex();

    ASSERT_EQ(statuses.size(), 1);
    EXPECT_EQ(statuses[0].eventType, IndexingEventType::WARNING);
    EXPECT_EQ(statuses[0].message, "Source path is empty");
    EXPECT_EQ(index.getDocumentCount(), 0);
}
