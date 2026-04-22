//
// Created by Nikita Koshelenko on 12.03.2026.
//

#ifndef RTUMIREA_PCS_KR2_SEARCH_ENGINE_SEARCHER_H
#define RTUMIREA_PCS_KR2_SEARCH_ENGINE_SEARCHER_H

#include "../InvertedIndex/InvertedIndex.h"


class Searcher {
private:
    InvertedIndex& index;

public:
    Searcher(InvertedIndex &idx);

    std::vector<std::string> search(const std::string& query);
};


#endif //RTUMIREA_PCS_KR2_SEARCH_ENGINE_SEARCHER_H
