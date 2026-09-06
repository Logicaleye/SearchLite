#include "InvertedIndex.h"

using namespace std;

void InvertedIndex::add(const string& term, int documentId) {
    auto& documents = index[term];

    // Prevent duplicate document IDs for the same term
    if (documents.empty() || documents.back() != documentId) {
        documents.push_back(documentId);
    }
}

vector<int> InvertedIndex::search(const string& term) const {
    auto it = index.find(term);

    if (it == index.end()) {
        return {};
    }

    return it->second;
}

bool InvertedIndex::contains(const string& term) const {
    return index.find(term) != index.end();
}

size_t InvertedIndex::size() const {
    return index.size();
}