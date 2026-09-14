#include "InvertedIndex.h"
#include "QueryProcessor.h"
#include "Ranker.h"
#include "TextProcessor.h"

#include <cassert>
#include <iostream>

int main() {

    TextProcessor textProcessor;
    InvertedIndex index;

    // Document 1: machine × 5
    for (int i = 0; i < 5; ++i) {
        index.add("machine", 1);
    }

    // Document 2: machine × 4
    for (int i = 0; i < 4; ++i) {
        index.add("machine", 2);
    }

    // Document 3: machine × 3
    for (int i = 0; i < 3; ++i) {
        index.add("machine", 3);
    }

    // Document 4: machine × 2
    for (int i = 0; i < 2; ++i) {
        index.add("machine", 4);
    }

    // Document 5: machine × 1
    index.add("machine", 5);

    // Document 6: does not contain machine
    index.add("computer", 6);

    QueryProcessor queryProcessor(
        index,
        textProcessor
    );

    Ranker ranker(
        index,
        textProcessor,
        queryProcessor
    );

    // --------------------------------------------
    // Test 1: Top 3
    // --------------------------------------------

    auto results =
        ranker.rank("machine", 3);

    assert(results.size() == 3);

    assert(results[0].documentId == 1);
    assert(results[1].documentId == 2);
    assert(results[2].documentId == 3);

    // --------------------------------------------
    // Test 2: Top 1
    // --------------------------------------------

    auto topOne =
        ranker.rank("machine", 1);

    assert(topOne.size() == 1);
    assert(topOne[0].documentId == 1);

    // --------------------------------------------
    // Test 3: Top K larger than results
    // --------------------------------------------

    auto allResults =
        ranker.rank("machine", 100);

    assert(allResults.size() == 5);

    assert(allResults[0].documentId == 1);
    assert(allResults[1].documentId == 2);
    assert(allResults[2].documentId == 3);
    assert(allResults[3].documentId == 4);
    assert(allResults[4].documentId == 5);

    // --------------------------------------------
    // Test 4: K = 0
    // --------------------------------------------

    auto zeroResults =
        ranker.rank("machine", 0);

    assert(zeroResults.empty());

    // --------------------------------------------
    // Test 5: Unknown query
    // --------------------------------------------

    auto unknownResults =
        ranker.rank("quantum", 3);

    assert(unknownResults.empty());

    // --------------------------------------------
    // Test 6: Empty query
    // --------------------------------------------

    auto emptyResults =
        ranker.rank("", 3);

    assert(emptyResults.empty());

    // --------------------------------------------
// Test 7: Deterministic tie-breaking
// --------------------------------------------

InvertedIndex tieIndex;

// "common" appears in documents 1, 2, 3
// with exactly the same frequency.

tieIndex.add("common", 1);
tieIndex.add("common", 2);
tieIndex.add("common", 3);

// Document 4 makes IDF positive.
tieIndex.add("other", 4);

QueryProcessor tieQueryProcessor(
    tieIndex,
    textProcessor
);

Ranker tieRanker(
    tieIndex,
    textProcessor,
    tieQueryProcessor
);

auto tieResults =
    tieRanker.rank("common", 2);

assert(tieResults.size() == 2);

// Same score → smaller document ID first.
assert(tieResults[0].documentId == 1);
assert(tieResults[1].documentId == 2);

    std::cout
        << "All Top-K tests passed!\n";

    return 0;
}