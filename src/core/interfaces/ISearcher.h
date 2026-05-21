#ifndef STSE_I_SEARCHER_H
#define STSE_I_SEARCHER_H

#include <string>
#include <vector>

#include "../model/SearchResult.h"

class ISearcher {
public:
    virtual ~ISearcher() = default;
    virtual std::vector<SearchResult> search(const std::string& query) = 0;
};

#endif
