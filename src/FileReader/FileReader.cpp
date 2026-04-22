//
// Created by Nikita Koshelenko on 12.03.2026.
//

#include "FileReader.h"

std::vector<std::string> FileReader::listFiles(const std::string &path) {
    std::vector<std::string> files = { "test.txt" };
    return files;
}

std::string FileReader::readFile(const std::string &path) {
    return "Hello world, test 123 — if work then WORK";
}
