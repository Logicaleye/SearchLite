#include "QueryProcessor.h"

#include <algorithm>
#include <unordered_set>
#include <iterator>

QueryProcessor::QueryProcessor(
    const InvertedIndex& index,
    const TextProcessor& textProcessor
)
    : index(index),
      textProcessor(textProcessor) {
}

std::vector<int> QueryProcessor::search(
    const std::string& query
) const {

    // Convert the query into searchable terms
    std::vector<std::string> terms =
        textProcessor.process(query);

    // Empty query
    if (terms.empty()) {
        return {};
    }

    // Start with documents matching the first term
    std::vector<int> results =
        index.search(terms[0]);

    // Intersect with results from remaining terms
    for (std::size_t i = 1; i < terms.size(); ++i) {

        std::vector<int> current =
            index.search(terms[i]);

        std::vector<int> intersection;

        std::set_intersection(
            results.begin(),
            results.end(),
            current.begin(),
            current.end(),
            std::back_inserter(intersection)
        );

        results = std::move(intersection);

        // No documents can match anymore
        if (results.empty()) {
            break;
        }
    }

    return results;
}