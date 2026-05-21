#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

#include "core/FileReader.h"
#include "core/Indexer.h"
#include "core/InvertedIndex.h"
#include "core/Searcher.h"
#include "core/Tokenizer.h"

namespace {

class TempDir {
public:
    TempDir() {
        const auto suffix = std::to_string(std::random_device{}());
        path_ = std::filesystem::temp_directory_path() / ("stse-e2e-" + suffix);
        std::filesystem::create_directories(path_);
    }

    ~TempDir() {
        std::error_code ec;
        std::filesystem::remove_all(path_, ec);
    }

    const std::filesystem::path& path() const {
        return path_;
    }

private:
    std::filesystem::path path_;
};

void writeTextFile(const std::filesystem::path& path, const std::string& content) {
    std::ofstream out(path);
    out << content;
}

std::unordered_map<std::string, int> buildFrequencyByFilename(const std::vector<SearchResult>& results) {
    std::unordered_map<std::string, int> byName;
    for (const auto& result : results) {
        byName[std::filesystem::path(result.path).filename().string()] = result.frequency;
    }
    return byName;
}

}

TEST(E2ETest, HappyPathDirectorySelectionIndexingAndExactSearch) {
    TempDir tmp;
    writeTextFile(tmp.path() / "doc1.txt", "search engine search");
    writeTextFile(tmp.path() / "doc2.txt", "search ranking");
    writeTextFile(tmp.path() / "ignored.md", "search");

    FileReader fileReader;
    Tokenizer tokenizer;
    InvertedIndex invertedIndex;
    Indexer indexer(&fileReader, &tokenizer, &invertedIndex);
    Searcher searcher(&tokenizer, &invertedIndex);

    std::vector<IndexingStatus> statuses;
    indexer.setStatusCallback([&statuses](const IndexingStatus& status) { statuses.push_back(status); });
    indexer.setSourcePath(tmp.path().string());
    indexer.buildIndex();

    ASSERT_FALSE(statuses.empty());
    EXPECT_EQ(statuses.front().eventType, IndexingEventType::STARTED);
    EXPECT_EQ(statuses.back().eventType, IndexingEventType::FINISHED);
    EXPECT_EQ(statuses.back().processedFiles, 2);

    const auto results = searcher.search("search");
    ASSERT_EQ(results.size(), 2);
    EXPECT_EQ(results[0].matchType, MatchType::EXACT);
    EXPECT_EQ(results[1].matchType, MatchType::EXACT);

    const auto byFilename = buildFrequencyByFilename(results);
    ASSERT_EQ(byFilename.size(), 2);
    EXPECT_EQ(byFilename.at("doc1.txt"), 2);
    EXPECT_EQ(byFilename.at("doc2.txt"), 1);
}
