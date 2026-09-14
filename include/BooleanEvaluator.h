#ifndef BOOLEAN_EVALUATOR_H
#define BOOLEAN_EVALUATOR_H

#include "BooleanParser.h"
#include "InvertedIndex.h"
#include "TextProcessor.h"

#include <vector>

class BooleanEvaluator {
private:
    const InvertedIndex& index;
    const TextProcessor& textProcessor;

    std::vector<int> evaluateNode(
        const BooleanNode* node
    ) const;

    std::vector<int> setAnd(
        const std::vector<int>& left,
        const std::vector<int>& right
    ) const;

    std::vector<int> setOr(
        const std::vector<int>& left,
        const std::vector<int>& right
    ) const;

    std::vector<int> setNot(
        const std::vector<int>& documents
    ) const;

public:
    BooleanEvaluator(
    const InvertedIndex& index,
    const TextProcessor& textProcessor
    );

    std::vector<int> evaluate(
        const BooleanNode* root
    ) const;
};

#endif