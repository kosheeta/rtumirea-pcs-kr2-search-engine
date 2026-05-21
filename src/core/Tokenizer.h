#ifndef STSE_TOKENIZER_H
#define STSE_TOKENIZER_H

#include "interfaces/ITokenizer.h"

class Tokenizer : public ITokenizer {
public:
    std::vector<std::string> tokenize(const std::string& text) override;
};

#endif
