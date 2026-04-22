//
// Created by Nikita Koshelenko on 12.03.2026.
//

#include "Tokenizer.h"

std::vector<std::string> Tokenizer::tokenize(const std::string &text) {
    std::vector<std::string> tokens;
    size_t pos = 0, len = 0;
    std::string token;
    while (pos < text.length()) {
        char c = text[pos];
        if (c == ' ' || std::ispunct(c)) {
            if (len > 0) {
                token = text.substr(pos - len, len);
                std::transform(token.begin(), token.end(), token.begin(),
                               [](unsigned char c) { return std::tolower(c); });
                tokens.push_back(token);
                len = 0;
            }
        } else {
            len++;
        }
        pos++;
    }

    token = text.substr(pos - len, len);
    std::transform(token.begin(), token.end(), token.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    tokens.push_back(token);

    return tokens;

//    for (char& c : cleanText) {
//        c = (char) std::tolower((int) c);
//        if (std::ispunct(c)) {
//            c = ' ';
//        }
//    }
//
//    std::vector<std::string> tokens;
//    size_t pos = 0;
//    std::string token;
//    while ((pos = s.find(' ')) != std::string::npos) {
//        token = s.substr(0, pos);
//        tokens.push_back(token);
//        s.erase(0, pos + 1);
//    }
//    tokens.push_back(s);
//
//    return tokens;
}
