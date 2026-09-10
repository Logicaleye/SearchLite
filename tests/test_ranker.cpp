#include "InvertedIndex.h"
#include "Ranker.h"
#include "TextProcessor.h"

#include <cassert>
#include <iostream>

int main() {
    InvertedIndex index;
    TextProcessor textProcessor;

    // Document 1
    index.add("machine", 1);
    index.add("machine", 1);
    index.add("machine", 1);

    index.add("learning", 1);
    index.add("learning", 1);
    index.add("learning", 1);

    // Document 2
    index.add("machine", 2);

    index.add("learning", 2);

    // Document 3
    index.add("machine", 3);
    index.add("learning", 3);

    QueryProcessor processor(index, textProcessor);
    Ranker ranker(index, textProcessor, processor);

    // Test 1: Higher frequency should produce higher score
    auto results = ranker.rank("machine");

    assert(results.size() == 3);

    assert(results[0].documentId == 1);
    assert(results[0].score == 3);

    assert(results[1].documentId == 2);
    assert(results[1].score == 1);

    assert(results[2].documentId == 3);
    assert(results[2].score == 1);

    // Test 2: Multiple query terms should accumulate scores
    auto multiTermResults = ranker.rank("machine learning");

    assert(multiTermResults.size() == 3);

    assert(multiTermResults[0].documentId == 1);
    assert(multiTermResults[0].score == 6);

    assert(multiTermResults[1].documentId == 2);
    assert(multiTermResults[1].score == 2);

    assert(multiTermResults[2].documentId == 3);
    assert(multiTermResults[2].score == 2);

    // Test 3: Unknown query
    auto unknownResults = ranker.rank("quantum");

    assert(unknownResults.empty());

    // Test 4: Empty query
    auto emptyResults = ranker.rank("");

    assert(emptyResults.empty());

        // Test 5: AND semantics
    auto andResults = ranker.rank("machine quantum");

    assert(andResults.empty());

        // Test 6: Documents must contain all query terms
    auto strictAndResults = ranker.rank("machine learning");

    assert(strictAndResults.size() == 3);

    assert(strictAndResults[0].documentId == 1);
    assert(strictAndResults[0].score == 6);
    
    std::cout << "All Ranker tests passed!\n";

    return 0;
}