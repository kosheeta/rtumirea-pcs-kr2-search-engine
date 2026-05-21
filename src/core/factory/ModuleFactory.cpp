#include "ModuleFactory.h"

#include "../FileReader.h"
#include "../Indexer.h"
#include "../InvertedIndex.h"
#include "../Searcher.h"
#include "../Tokenizer.h"

IFileReader* ModuleFactory::createFileReader() {
    return new FileReader();
}

ITokenizer* ModuleFactory::createTokenizer() {
    return new Tokenizer();
}

IInvertedIndex* ModuleFactory::createInvertedIndex() {
    return new InvertedIndex();
}

IIndexer* ModuleFactory::createIndexer(IFileReader* fileReader,
                                       ITokenizer* tokenizer,
                                       IInvertedIndex* invertedIndex) {
    return new Indexer(fileReader, tokenizer, invertedIndex);
}

ISearcher* ModuleFactory::createSearcher(ITokenizer* tokenizer,
                                         IInvertedIndex* invertedIndex) {
    return new Searcher(tokenizer, invertedIndex);
}
