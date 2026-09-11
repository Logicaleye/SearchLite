#include "InvertedIndex.h"
#include "QueryProcessor.h"
#include "TextProcessor.h"

#include <cassert>
#include <iostream>

int main() {

    TextProcessor textProcessor;
    InvertedIndex index;

    // Document 1:
    //
    // machine learning algorithms
    // machine learning
    //
    index.add("machine", 1, 0);
    index.add("learning", 1, 1);
    index.add("algorithms", 1, 2);

    index.add("machine", 1, 3);
    index.add("learning", 1, 4);

    // Document 2:
    //
    // machine learning [gap] algorithms
    //
    // machine and algorithms are NOT consecutive.
    //
    index.add("machine", 2, 0);
    index.add("learning", 2, 1);
    index.add("algorithms", 2, 3);

    QueryProcessor queryProcessor(
        index,
        textProcessor
    );

    // ------------------------------------------------
    // 1. Exact phrase
    // ------------------------------------------------

    auto machineLearning =
        queryProcessor.searchPhrase(
            "machine learning"
        );

    assert(machineLearning.size() == 2);
    assert(machineLearning[0] == 1);
    assert(machineLearning[1] == 2);

    // ------------------------------------------------
    // 2. Reversed order should NOT match
    // ------------------------------------------------

    auto learningMachine =
        queryProcessor.searchPhrase(
            "learning machine"
        );

    assert(learningMachine.empty());

    // ------------------------------------------------
    // 3. Non-consecutive terms should NOT match
    // ------------------------------------------------

    auto machineAlgorithms =
        queryProcessor.searchPhrase(
            "machine algorithms"
        );

    assert(machineAlgorithms.empty());

    // ------------------------------------------------
    // 4. Three-word phrase
    // ------------------------------------------------

    auto fullPhrase =
        queryProcessor.searchPhrase(
            "machine learning algorithms"
        );

    assert(fullPhrase.size() == 1);
    assert(fullPhrase[0] == 1);

    // ------------------------------------------------
    // 5. Unknown phrase
    // ------------------------------------------------

    auto unknown =
        queryProcessor.searchPhrase(
            "quantum computing"
        );

    assert(unknown.empty());

    // ------------------------------------------------
    // 6. Single-term search
    // ------------------------------------------------

    auto singleTerm =
        queryProcessor.searchPhrase(
            "machine"
        );

    assert(singleTerm.size() == 2);
    assert(singleTerm[0] == 1);
    assert(singleTerm[1] == 2);

    std::cout
        << "All phrase search tests passed!\n";

    return 0;
}