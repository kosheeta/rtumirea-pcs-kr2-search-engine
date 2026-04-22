//
// Created by Nikita Koshelenko on 12.03.2026.
//

#ifndef RTUMIREA_PCS_KR2_SEARCH_ENGINE_TOKENIZER_H
#define RTUMIREA_PCS_KR2_SEARCH_ENGINE_TOKENIZER_H

#include <iostream>


class Tokenizer {
public:
    static std::vector<std::string> tokenize(const std::string& text);
};


#endif //RTUMIREA_PCS_KR2_SEARCH_ENGINE_TOKENIZER_H
