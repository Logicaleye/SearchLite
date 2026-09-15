#include "Ranker.h"

#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_map>

double calculateIDF(
    const InvertedIndex& index,
    const std::string& term
) {
    const auto& postings =
        index.getPostings(term);

    if (postings.empty()) {
        return 0.0;
    }

    double totalDocuments =
        static_cast<double>(
            index.getAllDocuments().size()
        );

    double documentFrequency =
        static_cast<double>(
            postings.size()
        );

    return std::log(
        totalDocuments / documentFrequency
    );
}

struct WorseResultFirst {
    bool operator()(
        const RankedResult& a,
        const RankedResult& b
    ) const {

        if (a.score != b.score) {
            return a.score > b.score;
        }

        return a.documentId > b.documentId;
    }
};

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
    auto matchingDocuments =
    queryProcessor.searchCandidates(query);

    if (matchingDocuments.empty()) {
        return {};
    }

    // Store score for each matching document.
    std::unordered_map<int, double> scores;

    for (int documentId : matchingDocuments) {
        scores[documentId] = 0;
    }

    // Calculate TF-IDF score only for matching documents.
    for (const std::string& term : terms) {

    const auto& postings =
        index.getPostings(term);

    double idf =
        calculateIDF(index, term);

    for (const auto& [documentId, frequency] : postings) {

        if (scores.find(documentId) != scores.end()) {

            scores[documentId] +=
                frequency * idf;
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

std::vector<RankedResult> Ranker::rank(
    const std::string& query,
    std::size_t topK
) const {

    if (topK == 0) {
        return {};
    }

    std::vector<std::string> terms =
        textProcessor.process(query);

    if (terms.empty()) {
        return {};
    }

    auto matchingDocuments =
    queryProcessor.searchCandidates(query);

    if (matchingDocuments.empty()) {
        return {};
    }

    std::unordered_map<int, double> scores;

    for (int documentId : matchingDocuments) {
        scores[documentId] = 0.0;
    }

    for (const std::string& term : terms) {

        const auto& postings =
            index.getPostings(term);

        double idf =
            calculateIDF(index, term);

        for (const auto& [documentId, frequency] : postings) {

            if (scores.find(documentId) != scores.end()) {

                scores[documentId] +=
                    frequency * idf;
            }
        }
    }

    std::priority_queue<
        RankedResult,
        std::vector<RankedResult>,
        WorseResultFirst
    > heap;

    for (const auto& [documentId, score] : scores) {

        RankedResult result{
            documentId,
            score
        };

        if (heap.size() < topK) {

            heap.push(result);

        } else {

    const auto& worst = heap.top();

    bool better =
        result.score > worst.score ||
        (
            result.score == worst.score &&
            result.documentId < worst.documentId
        );

    if (better) {
        heap.pop();
        heap.push(result);
    }
}
    }

    std::vector<RankedResult> results;

    while (!heap.empty()) {

        results.push_back(heap.top());
        heap.pop();
    }

    std::sort(
        results.begin(),
        results.end(),
        [](const RankedResult& a,
           const RankedResult& b) {

            if (a.score != b.score) {
                return a.score > b.score;
            }

            return a.documentId < b.documentId;
        }
    );

    return results;
}