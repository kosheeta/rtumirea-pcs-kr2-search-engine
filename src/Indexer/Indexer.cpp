//
// Created by Nikita Koshelenko on 12.03.2026.
//

#include "Indexer.h"
#include "../FileReader/FileReader.h"
#include "../Tokenizer/Tokenizer.h"

void Indexer::addPath(const std::string &path) {
    std::vector<std::string> files = FileReader::listFiles(path);
    this->files.insert(this->files.end(), files.begin(), files.end());
}

void Indexer::buildIndex() {
    for (const std::string& filePath : this->files) {
        std::string text = FileReader::readFile(filePath);
        std::vector<std::string> tokens = Tokenizer::tokenize(text);

        for (const std::string& token : tokens) {
            this->index.addToken(token, filePath);
        }
    }
}
