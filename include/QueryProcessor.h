#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include "InvertedIndex.h"
#include "TextProcessor.h"
#include "BooleanParser.h"
#include "BooleanEvaluator.h"

#include <string>
#include <vector>

class QueryProcessor {
private:
    const InvertedIndex& index;
    const TextProcessor& textProcessor;

    mutable BooleanParser booleanParser;
    BooleanEvaluator booleanEvaluator;

public:
    QueryProcessor(
        const InvertedIndex& index,
        const TextProcessor& textProcessor
    );

    std::vector<int> search(
        const std::string& query
    ) const;

    std::vector<int> searchPhrase(
        const std::string& phrase
    ) const;

    std::vector<int> searchBoolean(
        const std::string& query
    ) const;

    bool isBooleanQuery(
    const std::string& query
    ) const;

    std::vector<int> searchCandidates(
    const std::string& query
    ) const;
};

#endif