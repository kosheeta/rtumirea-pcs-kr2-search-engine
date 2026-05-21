#include <filesystem>
#include <random>
#include <string>
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
        path_ = std::filesystem::temp_directory_path() / ("stse-e2e-empty-" + suffix);
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

}

TEST(E2ETest, EmptyDirectoryNoCrashAndEmptyResults) {
    TempDir tmp;

    FileReader fileReader;
    Tokenizer tokenizer;
    InvertedIndex invertedIndex;
    Indexer indexer(&fileReader, &tokenizer, &invertedIndex);
    Searcher searcher(&tokenizer, &invertedIndex);

    std::vector<IndexingStatus> statuses;
    indexer.setStatusCallback([&statuses](const IndexingStatus& status) { statuses.push_back(status); });
    indexer.setSourcePath(tmp.path().string());
    indexer.buildIndex();

    ASSERT_EQ(statuses.size(), 2);
    EXPECT_EQ(statuses[0].eventType, IndexingEventType::STARTED);
    EXPECT_EQ(statuses[1].eventType, IndexingEventType::FINISHED);
    EXPECT_EQ(statuses[1].processedFiles, 0);
    EXPECT_EQ(statuses[1].totalFiles, 0);
    EXPECT_EQ(statuses[1].uniqueTokens, 0);

    EXPECT_EQ(invertedIndex.getDocumentCount(), 0);
    EXPECT_EQ(invertedIndex.getUniqueTokenCount(), 0);
    EXPECT_TRUE(searcher.search("search").empty());
}
