#include "BooleanEvaluator.h"

#include <algorithm>
#include <iterator>

BooleanEvaluator::BooleanEvaluator(
    const InvertedIndex& index,
    const TextProcessor& textProcessor
)
    : index(index),
      textProcessor(textProcessor) {
}

std::vector<int> BooleanEvaluator::setAnd(
    const std::vector<int>& left,
    const std::vector<int>& right
) const {

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

std::vector<int> BooleanEvaluator::setOr(
    const std::vector<int>& left,
    const std::vector<int>& right
) const {

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

std::vector<int> BooleanEvaluator::setNot(
    const std::vector<int>& documents
) const {

    std::vector<int> allDocuments =
        index.getAllDocuments();

    std::vector<int> result;

    std::set_difference(
        allDocuments.begin(),
        allDocuments.end(),
        documents.begin(),
        documents.end(),
        std::back_inserter(result)
    );

    return result;
}

std::vector<int>
BooleanEvaluator::evaluateNode(
    const BooleanNode* node
) const {

    if (!node) {
        return {};
    }

    if (
    node->type ==
    BooleanNode::Type::TERM
) {

    auto terms =
        textProcessor.process(node->value);

    if (terms.size() != 1) {
        return {};
    }

    return index.search(terms[0]);
}

    if (
        node->type ==
        BooleanNode::Type::AND
    ) {

        auto left =
            evaluateNode(node->left.get());

        auto right =
            evaluateNode(node->right.get());

        return setAnd(left, right);
    }

    if (
        node->type ==
        BooleanNode::Type::OR
    ) {

        auto left =
            evaluateNode(node->left.get());

        auto right =
            evaluateNode(node->right.get());

        return setOr(left, right);
    }

    if (
        node->type ==
        BooleanNode::Type::NOT
    ) {

        auto documents =
            evaluateNode(node->left.get());

        return setNot(documents);
    }

    return {};
}

std::vector<int>
BooleanEvaluator::evaluate(
    const BooleanNode* root
) const {

    if (!root) {
        return {};
    }

    return evaluateNode(root);
}