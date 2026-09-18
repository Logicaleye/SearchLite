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
      textProcessor(textProcessor),
      booleanParser(),
      booleanEvaluator(index, textProcessor),
      cache(100) {
}

std::vector<int> QueryProcessor::search(
    const std::string& query
) const {

    std::vector<int> cachedResults;

    if (cache.get(query, cachedResults)) {
        return cachedResults;
    }

    std::vector<std::string> terms =
        textProcessor.process(query);

    if (terms.empty()) {
        return {};
    }

    std::vector<int> result =
        index.search(terms[0]);

    for (std::size_t i = 1;
         i < terms.size();
         ++i) {

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

    cache.put(query, result);

    return result;
}

std::vector<int>
QueryProcessor::searchPhrase(
    const std::string& phrase
) const {

    std::vector<int> cachedResults;

    if (cache.get(phrase, cachedResults)) {
        return cachedResults;
    }

    std::vector<std::string> terms =
        textProcessor.process(phrase);

    std::vector<int> results =
        index.searchPhrase(terms);

    cache.put(
        phrase,
        results
    );

    return results;
}

std::vector<int>
QueryProcessor::searchBoolean(
    const std::string& query
) const {

    std::vector<int> cachedResults;

    if (cache.get(query, cachedResults)) {
        return cachedResults;
    }

    auto root =
        booleanParser.parse(query);

    if (!root) {
        return {};
    }

    std::vector<int> results =
        booleanEvaluator.evaluate(
            root.get()
        );

    cache.put(
        query,
        results
    );

    return results;
}

bool QueryProcessor::isBooleanQuery(
    const std::string& query
) const {

    return booleanParser.isBooleanQuery(query);
}

std::vector<int>
QueryProcessor::searchCandidates(
    const std::string& query
) const {

    if (isBooleanQuery(query)) {
        return searchBoolean(query);
    }

    return search(query);
}