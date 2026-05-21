#include "FileReader.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>

bool isTextFile(const std::filesystem::path& path) {
    auto extension = path.extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return extension == ".txt";
}

std::vector<std::string> FileReader::listFiles(const std::string& path) {
    std::vector<std::string> files;

    std::error_code errorCode;
    const std::filesystem::path root(path);

    if (!std::filesystem::exists(root, errorCode) || !std::filesystem::is_directory(root, errorCode)) {
        return files;
    }

    std::filesystem::recursive_directory_iterator iterator(
        root,
        std::filesystem::directory_options::skip_permission_denied,
        errorCode
    );
    std::filesystem::recursive_directory_iterator end;

    while (iterator != end) {
        if (errorCode) {
            errorCode.clear();
            iterator.increment(errorCode);
            continue;
        }

        const auto& entry = *iterator;
        if (entry.is_regular_file(errorCode) && !errorCode && isTextFile(entry.path())) {
            files.push_back(std::filesystem::absolute(entry.path(), errorCode).string());
            errorCode.clear();
        }

        iterator.increment(errorCode);
    }

    std::sort(files.begin(), files.end());
    return files;
}

std::string FileReader::readFile(const std::string& path) {
    std::ifstream stream(path);
    if (!stream.is_open()) {
        return {};
    }

    return {std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};
}
