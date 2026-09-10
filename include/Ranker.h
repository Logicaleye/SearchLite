#ifndef RANKER_H
#define RANKER_H

#include "InvertedIndex.h"
#include "RankedResult.h"
#include "TextProcessor.h"
#include "QueryProcessor.h"
#include <string>
#include <vector>

class Ranker {
private:
    const InvertedIndex& index;
    const TextProcessor& textProcessor;
    const QueryProcessor& queryProcessor;

public:
    Ranker(
        const InvertedIndex& index,
        const TextProcessor& textProcessor,
        const QueryProcessor& queryProcessor
    );

    std::vector<RankedResult> rank(
        const std::string& query
    ) const;
};

#endif