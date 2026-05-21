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
        path_ = std::filesystem::temp_directory_path() / ("stse-e2e-multi-" + suffix);
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

TEST(E2ETest, MultiTokenQueryReturnsUnionOfDocuments) {
    TempDir tmp;
    writeTextFile(tmp.path() / "alpha_doc.txt", "alpha system");
    writeTextFile(tmp.path() / "beta_doc.txt", "beta module beta");
    writeTextFile(tmp.path() / "both_doc.txt", "alpha beta alpha");

    FileReader fileReader;
    Tokenizer tokenizer;
    InvertedIndex invertedIndex;
    Indexer indexer(&fileReader, &tokenizer, &invertedIndex);
    Searcher searcher(&tokenizer, &invertedIndex);

    indexer.setSourcePath(tmp.path().string());
    indexer.buildIndex();

    const auto results = searcher.search("alpha beta");

    ASSERT_EQ(results.size(), 3);
    std::unordered_set<std::string> names;
    for (const auto& result : results) {
        EXPECT_EQ(result.matchType, MatchType::EXACT);
        names.insert(std::filesystem::path(result.path).filename().string());
    }

    EXPECT_TRUE(names.contains("alpha_doc.txt"));
    EXPECT_TRUE(names.contains("beta_doc.txt"));
    EXPECT_TRUE(names.contains("both_doc.txt"));

    EXPECT_EQ(std::filesystem::path(results.front().path).filename().string(), "both_doc.txt");
    EXPECT_EQ(results.front().frequency, 3);
}
