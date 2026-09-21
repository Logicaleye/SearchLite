#include "InvertedIndex.h"

#include <algorithm>

void InvertedIndex::add(
    const std::string& term,
    int documentId
) {
    index[term][documentId]++;
    documents.insert(documentId);
}

void InvertedIndex::add(
    const std::string& term,
    int documentId,
    int position
) {
    index[term][documentId]++;

    positions[term][documentId].push_back(position);

    documents.insert(documentId);
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

    std::sort(
        documents.begin(),
        documents.end()
    );

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

const std::unordered_map<int, std::vector<int>>&
InvertedIndex::getPositions(
    const std::string& term
) const {
    static const std::unordered_map<
        int,
        std::vector<int>
    > emptyPositions;

    auto it = positions.find(term);

    if (it == positions.end()) {
        return emptyPositions;
    }

    return it->second;
}

std::vector<int> InvertedIndex::getAllDocuments() const {
    return std::vector<int>(
        documents.begin(),
        documents.end()
    );
}

std::vector<int>
InvertedIndex::searchPhrase(
    const std::vector<std::string>& terms
) const {

    if (terms.empty()) {
        return {};
    }

    // Single-term phrase
    if (terms.size() == 1) {
        return search(terms[0]);
    }

    const auto& firstPositions =
        getPositions(terms[0]);

    if (firstPositions.empty()) {
        return {};
    }

    std::vector<int> results;

    for (const auto& [documentId, positions] :
         firstPositions) {

        bool matched = false;

        for (int position : positions) {

            bool sequenceFound = true;

            for (std::size_t i = 1;
                 i < terms.size();
                 ++i) {

                const auto& nextPositions =
                    getPositions(terms[i]);

                auto documentIt =
                    nextPositions.find(documentId);

                if (
                    documentIt ==
                    nextPositions.end()
                ) {
                    sequenceFound = false;
                    break;
                }

                if (
                    !std::binary_search(
                        documentIt->second.begin(),
                        documentIt->second.end(),
                        position +
                            static_cast<int>(i)
                    )
                ) {
                    sequenceFound = false;
                    break;
                }
            }

            if (sequenceFound) {
                matched = true;
                break;
            }
        }

        if (matched) {
            results.push_back(documentId);
        }
    }

    std::sort(
        results.begin(),
        results.end()
    );

    return results;
}

std::vector<std::string>
InvertedIndex::getTerms() const {

    std::vector<std::string> terms;

    for (const auto& [term, postings] : index) {
        terms.push_back(term);
    }

    std::sort(
        terms.begin(),
        terms.end()
    );

    return terms;
}

void InvertedIndex::restore(
    const std::string& term,
    int documentId,
    int frequency,
    const std::vector<int>& positions
) {

    index[term][documentId] =
        frequency;

    if (!positions.empty()) {
        this->positions[term][documentId] =
            positions;
    }

    documents.insert(documentId);
}

void InvertedIndex::replaceWith(
    const InvertedIndex& other
) {
    index = other.index;
    positions = other.positions;
    documents = other.documents;
}

void InvertedIndex::clear() {
    index.clear();
    positions.clear();
    documents.clear();
}