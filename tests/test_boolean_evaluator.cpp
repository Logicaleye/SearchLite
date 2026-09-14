#include "BooleanEvaluator.h"
#include "BooleanParser.h"
#include "InvertedIndex.h"

#include <cassert>
#include <iostream>

int main() {

    InvertedIndex index;
    TextProcessor textProcessor;
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

    // Document 4:
    // algorithms
    index.add("algorithms", 4);

    BooleanParser parser;
    BooleanEvaluator evaluator(
    index,
    textProcessor
    );

    // --------------------------------------------
    // machine AND learning
    // --------------------------------------------

    auto query1 =
        parser.parse(
            "machine AND learning"
        );

    auto result1 =
        evaluator.evaluate(query1.get());

    assert(result1.size() == 1);
    assert(result1[0] == 1);

    // --------------------------------------------
    // machine OR learning
    // --------------------------------------------

    auto query2 =
        parser.parse(
            "machine OR learning"
        );

    auto result2 =
        evaluator.evaluate(query2.get());

    assert(result2.size() == 3);
    assert(result2[0] == 1);
    assert(result2[1] == 2);
    assert(result2[2] == 3);

    // --------------------------------------------
    // NOT machine
    // --------------------------------------------

    auto query3 =
        parser.parse(
            "NOT machine"
        );

    auto result3 =
        evaluator.evaluate(query3.get());

    assert(result3.size() == 2);
    assert(result3[0] == 3);
    assert(result3[1] == 4);

    // --------------------------------------------
    // machine AND (learning OR algorithms)
    // --------------------------------------------

    auto query4 =
        parser.parse(
            "machine AND (learning OR algorithms)"
        );

    auto result4 =
        evaluator.evaluate(query4.get());

    assert(result4.size() == 1);
    assert(result4[0] == 1);

    // --------------------------------------------
    // (machine OR learning) AND computer
    // --------------------------------------------

    auto query5 =
        parser.parse(
            "(machine OR learning) AND computer"
        );

    auto result5 =
        evaluator.evaluate(query5.get());

    assert(result5.size() == 2);
    assert(result5[0] == 2);
    assert(result5[1] == 3);

    auto caseResults =
    evaluator.evaluate(
        parser.parse("Machine AND Learning").get()
    );

    assert(caseResults.size() == 1);
    assert(caseResults[0] == 1);

    std::cout
        << "All Boolean evaluator tests passed!\n";

    return 0;
}