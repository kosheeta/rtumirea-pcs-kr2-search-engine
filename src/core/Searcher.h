#ifndef STSE_SEARCHER_H
#define STSE_SEARCHER_H

#include "interfaces/IInvertedIndex.h"
#include "interfaces/ISearcher.h"
#include "interfaces/ITokenizer.h"

class Searcher : public ISearcher {
public:
    Searcher(ITokenizer* tokenizer,
             IInvertedIndex* invertedIndex);

    std::vector<SearchResult> search(const std::string& query) override;

private:
    ITokenizer* tokenizer_;
    IInvertedIndex* invertedIndex_;
};

#endif
