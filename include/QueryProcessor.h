#ifndef QUERY_PROCESSOR_H
#define QUERY_PROCESSOR_H

#include "InvertedIndex.h"
#include "TextProcessor.h"

#include <string>
#include <vector>

class QueryProcessor {
private:
    const InvertedIndex& index;
    const TextProcessor& textProcessor;

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
};

#endif