#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <vector>
#include <unordered_map>

class InvertedIndex {
private:
    // term -> document ID -> term frequency
    std::unordered_map<
        std::string,
        std::unordered_map<int, int>
    > index;

    // term -> document ID -> positions
    std::unordered_map<
        std::string,
        std::unordered_map<int, std::vector<int>>
    > positions;

public:
    // Add a term without position information
    void add(
        const std::string& term,
        int documentId
    );

    // Add a term with its position
    void add(
        const std::string& term,
        int documentId,
        int position
    );

    std::vector<int> search(
        const std::string& term
    ) const;

    bool contains(
        const std::string& term
    ) const;

    std::size_t size() const;

    const std::unordered_map<int, int>& getPostings(
        const std::string& term
    ) const;

    const std::unordered_map<int, std::vector<int>>&
    getPositions(
        const std::string& term
    ) const;
};

#endif