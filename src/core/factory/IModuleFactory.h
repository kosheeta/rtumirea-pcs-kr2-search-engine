#ifndef STSE_I_MODULE_FACTORY_H
#define STSE_I_MODULE_FACTORY_H

#include "../interfaces/IFileReader.h"
#include "../interfaces/IIndexer.h"
#include "../interfaces/IInvertedIndex.h"
#include "../interfaces/ISearcher.h"
#include "../interfaces/ITokenizer.h"

class IModuleFactory {
public:
    virtual ~IModuleFactory() = default;

    virtual IFileReader* createFileReader() = 0;
    virtual ITokenizer* createTokenizer() = 0;
    virtual IInvertedIndex* createInvertedIndex() = 0;
    virtual IIndexer* createIndexer(IFileReader* fileReader,
                                    ITokenizer* tokenizer,
                                    IInvertedIndex* invertedIndex) = 0;
    virtual ISearcher* createSearcher(ITokenizer* tokenizer,
                                      IInvertedIndex* invertedIndex) = 0;
};

#endif
