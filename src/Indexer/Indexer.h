//
// Created by Nikita Koshelenko on 12.03.2026.
//

#ifndef RTUMIREA_PCS_KR2_SEARCH_ENGINE_INDEXER_H
#define RTUMIREA_PCS_KR2_SEARCH_ENGINE_INDEXER_H

#include "../InvertedIndex/InvertedIndex.h"


class Indexer {
private:
    InvertedIndex& index;

    std::vector<std::string> files;

public:
    Indexer(InvertedIndex& idx) : index(idx) {};

    void addPath(const std::string& path);
    void buildIndex();
};


#endif //RTUMIREA_PCS_KR2_SEARCH_ENGINE_INDEXER_H
