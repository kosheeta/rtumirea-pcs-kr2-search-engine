#ifndef STSE_INDEXER_H
#define STSE_INDEXER_H

#include "interfaces/IFileReader.h"
#include "interfaces/IIndexer.h"
#include "interfaces/IInvertedIndex.h"
#include "interfaces/ITokenizer.h"

class Indexer : public IIndexer {
public:
    Indexer(IFileReader* fileReader,
            ITokenizer* tokenizer,
            IInvertedIndex* invertedIndex);

    void setSourcePath(const std::string& path) override;
    void setStatusCallback(IndexingStatusCallback callback) override;
    void buildIndex() override;

private:
    void notifyStatus(const IndexingStatus& status) const;

    IFileReader* fileReader_;
    ITokenizer* tokenizer_;
    IInvertedIndex* invertedIndex_;
    std::string sourcePath_;
    IndexingStatusCallback statusCallback_;
};

#endif
