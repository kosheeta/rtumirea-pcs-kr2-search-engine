#include <filesystem>
#include <fstream>
#include <random>
#include <string>
#include <unordered_set>

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
        path_ = std::filesystem::temp_directory_path() / ("stse-e2e-fuzzy-" + suffix);
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

}

TEST(E2ETest, TypoQueryFallsBackToFuzzyResults) {
    TempDir tmp;
    writeTextFile(tmp.path() / "a.txt", "search engine");
    writeTextFile(tmp.path() / "b.txt", "search ranking");
    writeTextFile(tmp.path() / "c.txt", "research topic");

    FileReader fileReader;
    Tokenizer tokenizer;
    InvertedIndex invertedIndex;
    Indexer indexer(&fileReader, &tokenizer, &invertedIndex);
    Searcher searcher(&tokenizer, &invertedIndex);

    indexer.setSourcePath(tmp.path().string());
    indexer.buildIndex();

    const auto results = searcher.search("serch");

    ASSERT_FALSE(results.empty());
    for (const auto& result : results) {
        EXPECT_EQ(result.matchType, MatchType::FUZZY);
        EXPECT_EQ(result.matchedToken, "search");
        EXPECT_EQ(result.distance, 1);
    }

    std::unordered_set<std::string> names;
    for (const auto& result : results) {
        names.insert(std::filesystem::path(result.path).filename().string());
    }
    EXPECT_TRUE(names.contains("a.txt"));
    EXPECT_TRUE(names.contains("b.txt"));
}
