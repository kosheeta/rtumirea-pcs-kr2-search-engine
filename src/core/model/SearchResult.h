#ifndef STSE_SEARCH_RESULT_H
#define STSE_SEARCH_RESULT_H

#include <string>

enum class MatchType {
    EXACT,
    FUZZY
};

struct SearchResult {
    std::string path;
    MatchType matchType = MatchType::EXACT;
    std::string matchedToken;
    int frequency = 0;
    int distance = 0;
};

#endif
