#include "QueryProcessor.h"

#include <algorithm>
#include <iterator>
#include <cctype>

namespace {

std::vector<int> setAnd(
    const std::vector<int>& left,
    const std::vector<int>& right
) {
    std::vector<int> result;

    std::set_intersection(
        left.begin(),
        left.end(),
        right.begin(),
        right.end(),
        std::back_inserter(result)
    );

    return result;
}

std::vector<int> setOr(
    const std::vector<int>& left,
    const std::vector<int>& right
) {
    std::vector<int> result;

    std::set_union(
        left.begin(),
        left.end(),
        right.begin(),
        right.end(),
        std::back_inserter(result)
    );

    return result;
}

std::vector<int> setNot(
    const std::vector<int>& allDocuments,
    const std::vector<int>& excluded
) {
    std::vector<int> result;

    std::set_difference(
        allDocuments.begin(),
        allDocuments.end(),
        excluded.begin(),
        excluded.end(),
        std::back_inserter(result)
    );

    return result;
}

}

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

std::vector<int> QueryProcessor::searchBoolean(
    const std::string& query
) const {

    std::vector<std::string> tokens;

    std::string current;

    for (char ch : query) {

        if (std::isspace(
                static_cast<unsigned char>(ch))) {

            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }

        } else {
            current += ch;
        }
    }

    if (!current.empty()) {
        tokens.push_back(current);
    }

    // We currently support:
    //
    // term AND term
    // term OR term
    // term NOT term

    if (tokens.size() != 3) {
        return {};
    }

    std::string leftTerm =
        tokens[0];

    std::string operation =
        tokens[1];

    std::string rightTerm =
        tokens[2];

    // Process individual terms.
    auto leftTerms =
        textProcessor.process(leftTerm);

    auto rightTerms =
        textProcessor.process(rightTerm);

    if (leftTerms.size() != 1 ||
        rightTerms.size() != 1) {

        return {};
    }

    auto leftDocuments =
        index.search(leftTerms[0]);

    auto rightDocuments =
        index.search(rightTerms[0]);

    if (operation == "AND") {

        return setAnd(
            leftDocuments,
            rightDocuments
        );
    }

    if (operation == "OR") {

        return setOr(
            leftDocuments,
            rightDocuments
        );
    }

    if (operation == "NOT") {

        return setNot(
            leftDocuments,
            rightDocuments
        );
    }

    return {};
}