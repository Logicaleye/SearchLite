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

    // --------------------------------------------
// Nested Boolean expression
// machine AND (learning OR algorithms)
// --------------------------------------------

auto nestedResults =
    queryProcessor.searchBoolean(
        "machine AND (learning OR algorithms)"
    );

assert(nestedResults.size() == 1);
assert(nestedResults[0] == 1);


// --------------------------------------------
// Parentheses change precedence
// (machine OR learning) AND computer
// --------------------------------------------

auto groupedResults =
    queryProcessor.searchBoolean(
        "(machine OR learning) AND computer"
    );

assert(groupedResults.size() == 2);
assert(groupedResults[0] == 2);
assert(groupedResults[1] == 3);


// --------------------------------------------
// Operator precedence
// machine OR learning AND computer
// --------------------------------------------

auto precedenceResults =
    queryProcessor.searchBoolean(
        "machine OR learning AND computer"
    );

assert(precedenceResults.size() == 3);
assert(precedenceResults[0] == 1);
assert(precedenceResults[1] == 2);
assert(precedenceResults[2] == 3);


// --------------------------------------------
// Standalone NOT
// --------------------------------------------

auto standaloneNot =
    queryProcessor.searchBoolean(
        "NOT machine"
    );

assert(standaloneNot.size() == 1);
assert(standaloneNot[0] == 3);


// --------------------------------------------
// Lowercase operators
// --------------------------------------------

auto lowercaseResults =
    queryProcessor.searchBoolean(
        "machine and learning"
    );

assert(lowercaseResults.size() == 1);
assert(lowercaseResults[0] == 1);

    std::cout
        << "All Boolean search tests passed!\n";

    return 0;
}