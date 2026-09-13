#include "InvertedIndex.h"
#include "QueryProcessor.h"
#include "TextProcessor.h"

#include <cassert>
#include <iostream>

int main() {

    TextProcessor textProcessor;
    InvertedIndex index;

    // Document 1:
    // machine learning
    index.add("machine", 1);
    index.add("learning", 1);

    // Document 2:
    // machine computer
    index.add("machine", 2);
    index.add("computer", 2);

    // Document 3:
    // learning computer
    index.add("learning", 3);
    index.add("computer", 3);

    QueryProcessor queryProcessor(
        index,
        textProcessor
    );

    // --------------------------------------------
    // AND
    // --------------------------------------------

    auto andResults =
        queryProcessor.searchBoolean(
            "machine AND learning"
        );

    assert(andResults.size() == 1);
    assert(andResults[0] == 1);

    // --------------------------------------------
    // OR
    // --------------------------------------------

    auto orResults =
        queryProcessor.searchBoolean(
            "machine OR learning"
        );

    assert(orResults.size() == 3);
    assert(orResults[0] == 1);
    assert(orResults[1] == 2);
    assert(orResults[2] == 3);

    // --------------------------------------------
    // NOT
    // --------------------------------------------

    auto notResults =
        queryProcessor.searchBoolean(
            "machine NOT learning"
        );

    assert(notResults.size() == 1);
    assert(notResults[0] == 2);

    // --------------------------------------------
    // Unknown term
    // --------------------------------------------

    auto unknownResults =
        queryProcessor.searchBoolean(
            "machine AND quantum"
        );

    assert(unknownResults.empty());

    // --------------------------------------------
    // Invalid operator
    // --------------------------------------------

    auto invalidResults =
        queryProcessor.searchBoolean(
            "machine XOR learning"
        );

    assert(invalidResults.empty());

    std::cout
        << "All Boolean search tests passed!\n";

    return 0;
}