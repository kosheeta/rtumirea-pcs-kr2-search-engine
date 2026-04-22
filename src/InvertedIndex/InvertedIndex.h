//
// Created by Nikita Koshelenko on 12.03.2026.
//

#ifndef RTUMIREA_PCS_KR2_SEARCH_ENGINE_INVERTEDINDEX_H
#define RTUMIREA_PCS_KR2_SEARCH_ENGINE_INVERTEDINDEX_H

#include <iostream>

class InvertedIndex {
private:
    // { word: [(doc1, 1), (doc2, 3)] }
    std::unordered_map<std::string, std::vector<std::tuple<std::string, int>>> index;

public:
    void addToken(const std::string &token, const std::string doc);
    std::vector<std::pair<std::string, int>> search(const std::string& token);
};

#endif //RTUMIREA_PCS_KR2_SEARCH_ENGINE_INVERTEDINDEX_H
