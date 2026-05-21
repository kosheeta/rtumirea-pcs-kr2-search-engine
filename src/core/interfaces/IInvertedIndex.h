#ifndef STSE_I_INVERTED_INDEX_H
#define STSE_I_INVERTED_INDEX_H

#include <cstddef>
#include <string>
#include <vector>

struct Posting {
    std::string documentPath;
    int frequency;
};

class IInvertedIndex {
public:
    virtual ~IInvertedIndex() = default;
    virtual void clear() = 0;
    virtual void addToken(const std::string& token, const std::string& docPath) = 0;
    virtual std::vector<Posting> search(const std::string& token) const = 0;
    virtual std::vector<std::string> getAllTokens() const = 0;
    virtual std::size_t getDocumentCount() const = 0;
    virtual std::size_t getUniqueTokenCount() const = 0;
};

#endif
