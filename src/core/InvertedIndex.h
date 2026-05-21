#ifndef STSE_INVERTED_INDEX_H
#define STSE_INVERTED_INDEX_H

#include <cstddef>
#include <unordered_map>
#include <unordered_set>

#include "interfaces/IInvertedIndex.h"

class InvertedIndex : public IInvertedIndex {
public:
    void clear() override;
    void addToken(const std::string& token, const std::string& docPath) override;
    std::vector<Posting> search(const std::string& token) const override;
    std::vector<std::string> getAllTokens() const override;
    std::size_t getDocumentCount() const override;
    std::size_t getUniqueTokenCount() const override;

private:
    std::unordered_map<std::string, std::vector<Posting>> index_;
    std::unordered_set<std::string> documents_;
};

#endif
