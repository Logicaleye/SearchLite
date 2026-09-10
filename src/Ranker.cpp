#include "Ranker.h"

#include <algorithm>
#include <unordered_map>

Ranker::Ranker(
    const InvertedIndex& index,
    const TextProcessor& textProcessor,
    const QueryProcessor& queryProcessor
)
    : index(index),
      textProcessor(textProcessor),
      queryProcessor(queryProcessor) {
}

std::vector<RankedResult> Ranker::rank(
    const std::string& query
) const {
    std::vector<std::string> terms =
        textProcessor.process(query);

    if (terms.empty()) {
        return {};
    }

    // First determine which documents satisfy
    // the query using AND semantics.
    std::vector<int> matchingDocuments =
        queryProcessor.search(query);

    if (matchingDocuments.empty()) {
        return {};
    }

    // Store score for each matching document.
    std::unordered_map<int, int> scores;

    for (int documentId : matchingDocuments) {
        scores[documentId] = 0;
    }

    // Calculate TF score only for matching documents.
    for (const std::string& term : terms) {

        const auto& postings =
            index.getPostings(term);

        for (const auto& [documentId, frequency] : postings) {

            if (scores.find(documentId) != scores.end()) {
                scores[documentId] += frequency;
            }
        }
    }

    std::vector<RankedResult> results;

    for (const auto& [documentId, score] : scores) {
        results.push_back({documentId, score});
    }

    std::sort(
        results.begin(),
        results.end(),
        [](const RankedResult& a, const RankedResult& b) {

            if (a.score != b.score) {
                return a.score > b.score;
            }

            return a.documentId < b.documentId;
        }
    );

    return results;
}