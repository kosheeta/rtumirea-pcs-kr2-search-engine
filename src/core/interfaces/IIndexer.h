#ifndef STSE_I_INDEXER_H
#define STSE_I_INDEXER_H

#include <cstddef>
#include <functional>
#include <string>

enum class IndexingEventType {
    STARTED,
    PROGRESS,
    WARNING,
    FINISHED
};

struct IndexingStatus {
    IndexingEventType eventType;
    std::size_t processedFiles;
    std::size_t totalFiles;
    std::size_t uniqueTokens;
    std::string message;
};

using IndexingStatusCallback = std::function<void(const IndexingStatus&)>;

class IIndexer {
public:
    virtual ~IIndexer() = default;
    virtual void setSourcePath(const std::string& path) = 0;
    virtual void setStatusCallback(IndexingStatusCallback callback) = 0;
    virtual void buildIndex() = 0;
};

#endif
