#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <vector>
#include <unordered_map>

class InvertedIndex {
private:
    // term -> document ID -> frequency
    std::unordered_map<
        std::string,
        std::unordered_map<int, int>
    > index;

public:
    void add(const std::string& term, int documentId);

    std::vector<int> search(const std::string& term) const;

    bool contains(const std::string& term) const;

    std::size_t size() const;

    const std::unordered_map<int, int>& getPostings(
        const std::string& term
    ) const;
};

#endif