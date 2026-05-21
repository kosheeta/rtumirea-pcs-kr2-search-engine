#include "Searcher.h"

#include <algorithm>
#include <limits>
#include <unordered_map>

#include "config/SearchConfig.h"

namespace {

int levenshteinDistance(const std::string& left, const std::string& right) {
    if (left.empty()) {
        return static_cast<int>(right.size());
    }
    if (right.empty()) {
        return static_cast<int>(left.size());
    }

    std::vector<int> prev(right.size() + 1, 0);
    std::vector<int> curr(right.size() + 1, 0);

    for (std::size_t j = 0; j <= right.size(); ++j) {
        prev[j] = static_cast<int>(j);
    }

    for (std::size_t i = 1; i <= left.size(); ++i) {
        curr[0] = static_cast<int>(i);
        for (std::size_t j = 1; j <= right.size(); ++j) {
            const int substitutionCost = left[i - 1] == right[j - 1] ? 0 : 1;
            curr[j] = std::min({
                prev[j] + 1,
                curr[j - 1] + 1,
                prev[j - 1] + substitutionCost
            });
        }
        prev.swap(curr);
    }

    return prev[right.size()];
}

bool isStopWord(const std::string& token) {
    return SearchConfig::kStopWords.find(token) != SearchConfig::kStopWords.end();
}

void mergeExactResult(std::unordered_map<std::string, SearchResult>& byDocument,
                      const Posting& posting,
                      const std::string& matchedToken) {
    const auto it = byDocument.find(posting.documentPath);
    if (it == byDocument.end()) {
        byDocument.emplace(posting.documentPath,
                           SearchResult{posting.documentPath,
                                        MatchType::EXACT,
                                        matchedToken,
                                        posting.frequency,
                                        0});
        return;
    }

    it->second.frequency += posting.frequency;
}

void mergeFuzzyResult(std::unordered_map<std::string, SearchResult>& byDocument,
                      const Posting& posting,
                      const std::string& matchedToken,
                      int distance) {
    const auto it = byDocument.find(posting.documentPath);
    if (it == byDocument.end()) {
        byDocument.emplace(posting.documentPath,
                           SearchResult{posting.documentPath,
                                        MatchType::FUZZY,
                                        matchedToken,
                                        posting.frequency,
                                        distance});
        return;
    }

    if (distance < it->second.distance ||
        (distance == it->second.distance && posting.frequency > it->second.frequency)) {
        it->second.matchedToken = matchedToken;
        it->second.distance = distance;
        it->second.frequency = posting.frequency;
    }
}

}

Searcher::Searcher(ITokenizer* tokenizer,
                   IInvertedIndex* invertedIndex)
    : tokenizer_(tokenizer),
      invertedIndex_(invertedIndex) {
}

std::vector<SearchResult> Searcher::search(const std::string& query) {
    const auto rawQueryTokens = tokenizer_->tokenize(query);
    if (rawQueryTokens.empty()) {
        return {};
    }

    std::vector<std::string> queryTokens;
    queryTokens.reserve(rawQueryTokens.size());
    for (const auto& token : rawQueryTokens) {
        if (!isStopWord(token)) {
            queryTokens.push_back(token);
        }
    }

    if (queryTokens.empty()) {
        return {};
    }

    std::unordered_map<std::string, SearchResult> exactByDocument;
    std::unordered_map<std::string, SearchResult> fuzzyByDocument;
    const auto allTokens = invertedIndex_->getAllTokens();

    for (const auto& queryToken : queryTokens) {
        const auto exactPostings = invertedIndex_->search(queryToken);
        for (const auto& posting : exactPostings) {
            mergeExactResult(exactByDocument, posting, queryToken);
        }

        int minDistance = std::numeric_limits<int>::max();
        std::vector<std::string> nearestTokens;

        for (const auto& indexedToken : allTokens) {
            const int distance = levenshteinDistance(queryToken, indexedToken);
            if (distance > SearchConfig::kFuzzyThreshold) {
                continue;
            }

            if (distance < minDistance) {
                minDistance = distance;
                nearestTokens.clear();
                nearestTokens.push_back(indexedToken);
            } else if (distance == minDistance) {
                nearestTokens.push_back(indexedToken);
            }
        }

        for (const auto& nearestToken : nearestTokens) {
            const auto fuzzyPostings = invertedIndex_->search(nearestToken);
            for (const auto& posting : fuzzyPostings) {
                if (exactByDocument.find(posting.documentPath) != exactByDocument.end()) {
                    continue;
                }
                mergeFuzzyResult(fuzzyByDocument, posting, nearestToken, minDistance);
            }
        }
    }

    std::vector<SearchResult> results;
    results.reserve(exactByDocument.size() + fuzzyByDocument.size());

    for (const auto& [_, result] : exactByDocument) {
        results.push_back(result);
    }

    for (const auto& [_, result] : fuzzyByDocument) {
        results.push_back(result);
    }

    std::sort(results.begin(), results.end(), [](const SearchResult& left, const SearchResult& right) {
        if (left.matchType != right.matchType) {
            return left.matchType == MatchType::EXACT;
        }

        if (left.matchType == MatchType::EXACT) {
            if (left.frequency != right.frequency) {
                return left.frequency > right.frequency;
            }
            return left.path < right.path;
        }

        if (left.distance != right.distance) {
            return left.distance < right.distance;
        }
        if (left.frequency != right.frequency) {
            return left.frequency > right.frequency;
        }
        return left.path < right.path;
    });

    return results;
}
