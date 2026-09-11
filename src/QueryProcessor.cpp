#include "QueryProcessor.h"

#include <algorithm>
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
    std::vector<std::string> terms =
        textProcessor.process(query);

    if (terms.empty()) {
        return {};
    }

    std::vector<int> result =
        index.search(terms[0]);

    for (std::size_t i = 1; i < terms.size(); ++i) {

        std::vector<int> current =
            index.search(terms[i]);

        std::vector<int> intersection;

        std::set_intersection(
            result.begin(),
            result.end(),
            current.begin(),
            current.end(),
            std::back_inserter(intersection)
        );

        result = intersection;

        if (result.empty()) {
            break;
        }
    }

    return result;
}

std::vector<int> QueryProcessor::searchPhrase(
    const std::string& phrase
) const {
    std::vector<std::string> terms =
        textProcessor.process(phrase);

    if (terms.empty()) {
        return {};
    }

    // A single term does not need positional matching.
    if (terms.size() == 1) {
        return index.search(terms[0]);
    }

    // Start with documents containing the first term.
    std::vector<int> candidateDocuments =
        index.search(terms[0]);

    std::vector<int> results;

    for (int documentId : candidateDocuments) {

        const auto& firstPositions =
            index.getPositions(terms[0]);

        auto firstDocument =
            firstPositions.find(documentId);

        if (firstDocument == firstPositions.end()) {
            continue;
        }

        const std::vector<int>& positions =
            firstDocument->second;

        bool phraseFound = false;

        // Try every occurrence of the first term.
        for (int startPosition : positions) {

            bool matches = true;

            // Check whether every following term
            // occurs at the immediately following position.
            for (std::size_t i = 1;
                 i < terms.size();
                 ++i) {

                const auto& termPositions =
                    index.getPositions(terms[i]);

                auto documentPositions =
                    termPositions.find(documentId);

                if (documentPositions ==
                    termPositions.end()) {

                    matches = false;
                    break;
                }

                int expectedPosition =
                    startPosition +
                    static_cast<int>(i);

                if (!std::binary_search(
                        documentPositions->second.begin(),
                        documentPositions->second.end(),
                        expectedPosition)) {

                    matches = false;
                    break;
                }
            }

            if (matches) {
                phraseFound = true;
                break;
            }
        }

        if (phraseFound) {
            results.push_back(documentId);
        }
    }

    return results;
}