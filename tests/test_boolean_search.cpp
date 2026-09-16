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

    // --------------------------------------------
    // Semantic edge cases
    // --------------------------------------------

    // Unknown term with AND
    auto unknownAnd =
        queryProcessor.searchBoolean(
            "machine AND quantum"
        );

    assert(unknownAnd.empty());

    // Unknown term with OR
    auto unknownOr =
        queryProcessor.searchBoolean(
            "machine OR quantum"
        );

    assert(unknownOr.size() == 2);
    assert(unknownOr[0] == 1);
    assert(unknownOr[1] == 2);

    // NOT unknown should return every indexed document.
    auto notUnknown =
        queryProcessor.searchBoolean(
            "NOT quantum"
        );

    assert(notUnknown.size() == 3);
    assert(notUnknown[0] == 1);
    assert(notUnknown[1] == 2);
    assert(notUnknown[2] == 3);

    // Double NOT should cancel out.
    auto doubleNot =
        queryProcessor.searchBoolean(
            "NOT NOT machine"
        );

    assert(doubleNot.size() == 2);
    assert(doubleNot[0] == 1);
    assert(doubleNot[1] == 2);

    // Explicit AND NOT
    auto andNot =
        queryProcessor.searchBoolean(
            "machine AND NOT learning"
        );

    assert(andNot.size() == 1);
    assert(andNot[0] == 2);

    {
    InvertedIndex phraseIndex;

    // Document 1:
    // machine learning algorithms
    phraseIndex.add("machine", 1, 0);
    phraseIndex.add("learning", 1, 1);
    phraseIndex.add("algorithms", 1, 2);
    phraseIndex.add("computer", 1, 3);

    // Document 2:
    // machine algorithms learning
    phraseIndex.add("machine", 2, 0);
    phraseIndex.add("algorithms", 2, 1);
    phraseIndex.add("learning", 2, 2);
    phraseIndex.add("computer", 2, 3);

    // Document 3:
    // computer learning algorithms
    phraseIndex.add("computer", 3, 0);
    phraseIndex.add("learning", 3, 1);
    phraseIndex.add("algorithms", 3, 2);

    TextProcessor phraseTextProcessor;

    QueryProcessor phraseQueryProcessor(
        phraseIndex,
        phraseTextProcessor
    );

    // Test 1: Exact phrase
    auto result1 =
        phraseQueryProcessor.searchBoolean(
            "\"machine learning\""
        );

    assert(
        (result1 == std::vector<int>{1})
    );

    // Test 2: Phrase AND term
    auto result2 =
        phraseQueryProcessor.searchBoolean(
            "\"machine learning\" AND computer"
        );

    assert(
        (result2 == std::vector<int>{1})
    );

    // Test 3: Phrase OR term
    auto result3 =
        phraseQueryProcessor.searchBoolean(
            "\"machine learning\" OR computer"
        );

    assert(
        (result3 == std::vector<int>{1, 2, 3})
    );

    // Test 4: NOT phrase
    auto result4 =
        phraseQueryProcessor.searchBoolean(
            "NOT \"machine learning\""
        );

    assert(
        (result4 == std::vector<int>{2, 3})
    );

    // Test 5: Phrase must be consecutive
    auto result5 =
        phraseQueryProcessor.searchBoolean(
            "\"machine learning\""
        );

    assert(
        (result5 == std::vector<int>{1})
    );

    // Test 6: Phrase inside parentheses
    auto result6 =
        phraseQueryProcessor.searchBoolean(
            "(\"machine learning\" OR computer) AND algorithms"
        );

    assert(
        (result6 == std::vector<int>{1, 2, 3})
    );
}

    std::cout
        << "All Boolean search tests passed!\n";

    return 0;
}