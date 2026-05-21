#include "InvertedIndex.h"

void InvertedIndex::clear() {
    index_.clear();
    documents_.clear();
}

void InvertedIndex::addToken(const std::string& token, const std::string& docPath) {
    documents_.insert(docPath);

    auto& postings = index_[token];
    for (auto& posting : postings) {
        if (posting.documentPath == docPath) {
            posting.frequency++;
            return;
        }
    }

    postings.push_back({docPath, 1});
}

std::vector<Posting> InvertedIndex::search(const std::string& token) const {
    const auto it = index_.find(token);
    if (it == index_.end()) {
        return {};
    }

    return it->second;
}

std::vector<std::string> InvertedIndex::getAllTokens() const {
    std::vector<std::string> tokens;
    tokens.reserve(index_.size());

    for (const auto& [token, _] : index_) {
        tokens.push_back(token);
    }

    return tokens;
}

std::size_t InvertedIndex::getDocumentCount() const {
    return documents_.size();
}

std::size_t InvertedIndex::getUniqueTokenCount() const {
    return index_.size();
}
