#ifndef STSE_I_TOKENIZER_H
#define STSE_I_TOKENIZER_H

#include <string>
#include <vector>

class ITokenizer {
public:
    virtual ~ITokenizer() = default;
    virtual std::vector<std::string> tokenize(const std::string& text) = 0;
};

#endif
