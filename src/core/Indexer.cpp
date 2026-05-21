#include "Indexer.h"

Indexer::Indexer(IFileReader* fileReader,
                 ITokenizer* tokenizer,
                 IInvertedIndex* invertedIndex)
    : fileReader_(fileReader),
      tokenizer_(tokenizer),
      invertedIndex_(invertedIndex) {
}

void Indexer::setSourcePath(const std::string& path) {
    sourcePath_ = path;
}

void Indexer::setStatusCallback(IndexingStatusCallback callback) {
    statusCallback_ = std::move(callback);
}

void Indexer::notifyStatus(const IndexingStatus& status) const {
    if (statusCallback_) {
        statusCallback_(status);
    }
}

void Indexer::buildIndex() {
    invertedIndex_->clear();

    if (sourcePath_.empty()) {
        notifyStatus({
            .eventType = IndexingEventType::WARNING,
            .processedFiles = 0,
            .totalFiles = 0,
            .uniqueTokens = 0,
            .message = "Source path is empty"
        });
        return;
    }

    const auto files = fileReader_->listFiles(sourcePath_);
    notifyStatus({
        .eventType = IndexingEventType::STARTED,
        .processedFiles = 0,
        .totalFiles = files.size(),
        .uniqueTokens = 0,
        .message = "Indexing started"
    });

    std::size_t processedFiles = 0;

    for (const auto& file : files) {
        const auto text = fileReader_->readFile(file);
        const auto tokens = tokenizer_->tokenize(text);
        for (const auto& token : tokens) {
            invertedIndex_->addToken(token, file);
        }

        ++processedFiles;
        notifyStatus({
            .eventType = IndexingEventType::PROGRESS,
            .processedFiles = processedFiles,
            .totalFiles = files.size(),
            .uniqueTokens = invertedIndex_->getUniqueTokenCount(),
            .message = file
        });
    }

    notifyStatus({
        .eventType = IndexingEventType::FINISHED,
        .processedFiles = invertedIndex_->getDocumentCount(),
        .totalFiles = files.size(),
        .uniqueTokens = invertedIndex_->getUniqueTokenCount(),
        .message = "Indexing finished"
    });
}
