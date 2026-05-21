#ifndef STSE_SEARCH_CONFIG_H
#define STSE_SEARCH_CONFIG_H

#include <string_view>
#include <unordered_set>

namespace SearchConfig {

    const int kFuzzyThreshold = 2;

    const std::unordered_set<std::string_view> kStopWords = {
            // English
            "a", "an", "and", "are", "as", "at", "be", "but", "by",
            "for", "if", "in", "into", "is", "it", "no", "not",
            "of", "on", "or", "s", "such", "t", "that", "the",
            "their", "then", "there", "these", "they", "this",
            "to", "was", "will", "with",

            // Russian (search-oriented minimal set)
            "и", "в", "во", "на", "с", "со", "к", "ко", "у",
            "о", "об", "от", "до", "по", "за", "из", "под",
            "над", "при", "для", "без", "про",

            "а", "но", "или", "да", "не", "ни",

            "что", "это", "как", "так", "то", "же", "ли"
    };

}

#endif
