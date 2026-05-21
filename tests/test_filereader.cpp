#include <filesystem>
#include <fstream>
#include <random>

#include <gtest/gtest.h>

#include "core/FileReader.h"

namespace {

class TempDir {
public:
    TempDir() {
        const auto suffix = std::to_string(std::random_device{}());
        path_ = std::filesystem::temp_directory_path() / ("stse-" + suffix);
        std::filesystem::create_directories(path_);
    }

    ~TempDir() {
        std::error_code ec;
        std::filesystem::remove_all(path_, ec);
    }

    const std::filesystem::path& path() const { return path_; }

private:
    std::filesystem::path path_;
};

void writeTextFile(const std::filesystem::path& path, const std::string& content) {
    std::ofstream out(path);
    out << content;
}

}

TEST(FileReaderTest, ListsAllTxtFilesRecursivelyAsAbsolutePaths) {
    TempDir tmp;
    std::filesystem::create_directories(tmp.path() / "nested");

    writeTextFile(tmp.path() / "a.txt", "alpha");
    writeTextFile(tmp.path() / "nested" / "b.TXT", "beta");

    FileReader reader;
    const auto files = reader.listFiles(tmp.path().string());

    ASSERT_EQ(files.size(), 2);
    EXPECT_TRUE(std::filesystem::path(files[0]).is_absolute());
    EXPECT_TRUE(std::filesystem::path(files[1]).is_absolute());
}

TEST(FileReaderTest, IgnoresFilesWithOtherExtensions) {
    TempDir tmp;
    writeTextFile(tmp.path() / "a.txt", "alpha");
    writeTextFile(tmp.path() / "b.md", "beta");
    writeTextFile(tmp.path() / "c.log", "gamma");

    FileReader reader;
    const auto files = reader.listFiles(tmp.path().string());

    ASSERT_EQ(files.size(), 1);
    EXPECT_EQ(std::filesystem::path(files[0]).filename().string(), "a.txt");
}

TEST(FileReaderTest, ReturnsEmptyListForEmptyDirectory) {
    TempDir tmp;
    FileReader reader;

    EXPECT_TRUE(reader.listFiles(tmp.path().string()).empty());
}

TEST(FileReaderTest, ReturnsEmptyListForNonexistentDirectory) {
    FileReader reader;
    EXPECT_TRUE(reader.listFiles("/path/that/does/not/exist").empty());
}

TEST(FileReaderTest, ReadFileHandlesMissingFileAndStillReadsValidOne) {
    TempDir tmp;
    const auto validPath = tmp.path() / "ok.txt";
    writeTextFile(validPath, "search engine");

    FileReader reader;
    EXPECT_EQ(reader.readFile((tmp.path() / "missing.txt").string()), "");
    EXPECT_EQ(reader.readFile(validPath.string()), "search engine");
}
