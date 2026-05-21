#ifndef STSE_PRODUCTION_MODULE_FACTORY_H
#define STSE_PRODUCTION_MODULE_FACTORY_H

#include "IModuleFactory.h"

class ModuleFactory : public IModuleFactory {
public:
    IFileReader* createFileReader() override;
    ITokenizer* createTokenizer() override;
    IInvertedIndex* createInvertedIndex() override;
    IIndexer* createIndexer(IFileReader* fileReader,
                            ITokenizer* tokenizer,
                            IInvertedIndex* invertedIndex) override;
    ISearcher* createSearcher(ITokenizer* tokenizer,
                              IInvertedIndex* invertedIndex) override;
};

#endif
