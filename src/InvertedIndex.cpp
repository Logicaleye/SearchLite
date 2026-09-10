#include "InvertedIndex.h"
#include <algorithm>
void InvertedIndex::add(
    const std::string& term,
    int documentId
) {
    index[term][documentId]++;
}

std::vector<int> InvertedIndex::search(
    const std::string& term
) const {
    auto it = index.find(term);

    if (it == index.end()) {
        return {};
    }

    std::vector<int> documents;

    for (const auto& [documentId, frequency] : it->second) {
        documents.push_back(documentId);
    }

    std::sort(documents.begin(), documents.end());

    return documents;
}

bool InvertedIndex::contains(
    const std::string& term
) const {
    return index.find(term) != index.end();
}

std::size_t InvertedIndex::size() const {
    return index.size();
}

const std::unordered_map<int, int>&
InvertedIndex::getPostings(
    const std::string& term
) const {
    static const std::unordered_map<int, int> emptyPostings;

    auto it = index.find(term);

    if (it == index.end()) {
        return emptyPostings;
    }

    return it->second;
}