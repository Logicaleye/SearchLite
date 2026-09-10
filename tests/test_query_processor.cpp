#include "InvertedIndex.h"
#include "QueryProcessor.h"
#include "TextProcessor.h"

#include <cassert>
#include <iostream>

int main() {

    InvertedIndex index;
    TextProcessor textProcessor;

    // Document 1
    index.add("machine", 1);
    index.add("learning", 1);
    index.add("artificial", 1);
    index.add("intelligence", 1);

    // Document 2
    index.add("machine", 2);
    index.add("computer", 2);
    index.add("vision", 2);
    index.add("artificial", 2);
    index.add("intelligence", 2);

    // Document 3
    index.add("data", 3);
    index.add("structures", 3);
    index.add("algorithms", 3);

    QueryProcessor processor(index, textProcessor);

    // Test 1: Single term
    auto machineResults =
        processor.search("machine");

    assert(machineResults.size() == 2);
    assert(machineResults[0] == 1);
    assert(machineResults[1] == 2);

    // Test 2: Two-term AND query
    auto aiResults =
        processor.search("artificial intelligence");

    assert(aiResults.size() == 2);
    assert(aiResults[0] == 1);
    assert(aiResults[1] == 2);

    // Test 3: Terms that don't occur together
    auto machineVisionResults =
        processor.search("machine vision");

    assert(machineVisionResults.size() == 1);
    assert(machineVisionResults[0] == 2);

    // Test 4: Unknown term
    auto unknownResults =
        processor.search("quantum");

    assert(unknownResults.empty());

    // Test 5: Empty query
    auto emptyResults =
        processor.search("");

    assert(emptyResults.empty());

    std::cout << "All QueryProcessor tests passed!\n";

    return 0;
}