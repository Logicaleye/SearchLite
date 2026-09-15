#include "InvertedIndex.h"
#include "QueryProcessor.h"
#include "Ranker.h"
#include "TextProcessor.h"

#include <cassert>
#include <cmath>
#include <iostream>

bool nearlyEqual(double a, double b) {
    return std::abs(a - b) < 1e-9;
}

int main() {

    TextProcessor textProcessor;
    InvertedIndex index;

    // --------------------------------------------
    // Document 1
    // machine appears 3 times
    // learning appears 3 times
    // --------------------------------------------

    index.add("machine", 1);
    index.add("machine", 1);
    index.add("machine", 1);

    index.add("learning", 1);
    index.add("learning", 1);
    index.add("learning", 1);

    // --------------------------------------------
    // Document 2
    // machine appears once
    // learning appears once
    // --------------------------------------------

    index.add("machine", 2);
    index.add("learning", 2);

    // --------------------------------------------
    // Document 3
    // machine appears once
    // learning appears once
    // --------------------------------------------

    index.add("machine", 3);
    index.add("learning", 3);

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
    // Test 1: TF-IDF single term
    // --------------------------------------------

    auto results =
        ranker.rank("machine");

    assert(results.size() == 3);

    double idf =
        std::log(3.0 / 3.0);

    // machine appears in every document,
    // therefore IDF = 0.
    assert(nearlyEqual(idf, 0.0));

    assert(nearlyEqual(results[0].score, 0.0));
    assert(nearlyEqual(results[1].score, 0.0));
    assert(nearlyEqual(results[2].score, 0.0));

    // Tie-breaking should be document ID.
    assert(results[0].documentId == 1);
    assert(results[1].documentId == 2);
    assert(results[2].documentId == 3);

    // --------------------------------------------
    // Test 2: Rare term gets higher IDF
    // --------------------------------------------

    index.add("quantum", 1);

    // Re-create Ranker is unnecessary because
    // it holds references to the same index.

    auto quantumResults =
        ranker.rank("quantum");

    assert(quantumResults.size() == 1);
    assert(quantumResults[0].documentId == 1);

    double quantumIDF =
        std::log(3.0 / 1.0);

    assert(
        nearlyEqual(
            quantumResults[0].score,
            quantumIDF
        )
    );

    // --------------------------------------------
    // Test 3: Higher TF increases score
    // --------------------------------------------

    index.add("rare", 1);
    index.add("rare", 1);
    index.add("rare", 1);

    index.add("rare", 2);

    auto rareResults =
        ranker.rank("rare");

    assert(rareResults.size() == 2);

    // rare appears in 2 of 3 documents.
    double rareIDF =
        std::log(3.0 / 2.0);

    assert(
        nearlyEqual(
            rareResults[0].score,
            3.0 * rareIDF
        )
    );

    assert(
        nearlyEqual(
            rareResults[1].score,
            1.0 * rareIDF
        )
    );

    assert(rareResults[0].documentId == 1);
    assert(rareResults[1].documentId == 2);

    // --------------------------------------------
    // Test 4: Unknown term
    // --------------------------------------------

    auto unknownResults =
        ranker.rank("unknown");

    assert(unknownResults.empty());

    // --------------------------------------------
    // Test 5: Empty query
    // --------------------------------------------

    auto emptyResults =
        ranker.rank("");

    assert(emptyResults.empty());

    // --------------------------------------------
// Test 6: Multi-term TF-IDF
// --------------------------------------------

InvertedIndex multiIndex;

multiIndex.add("machine", 1);
multiIndex.add("machine", 1);
multiIndex.add("learning", 1);

multiIndex.add("machine", 2);
multiIndex.add("learning", 2);
multiIndex.add("learning", 2);

multiIndex.add("machine", 3);

QueryProcessor multiQueryProcessor(
    multiIndex,
    textProcessor
);

Ranker multiRanker(
    multiIndex,
    textProcessor,
    multiQueryProcessor
);

auto multiResults =
    multiRanker.rank("machine learning");

assert(multiResults.size() == 2);

double machineIDF =
    std::log(3.0 / 3.0);

double learningIDF =
    std::log(3.0 / 2.0);

// Document 1:
// machine × 2 + learning × 1
double expectedDoc1 =
    (2.0 * machineIDF) +
    (1.0 * learningIDF);

// Document 2:
// machine × 1 + learning × 2
double expectedDoc2 =
    (1.0 * machineIDF) +
    (2.0 * learningIDF);

assert(
    nearlyEqual(
        multiResults[0].score,
        expectedDoc2
    )
);

assert(
    nearlyEqual(
        multiResults[1].score,
        expectedDoc1
    )
);

assert(multiResults[0].documentId == 2);
assert(multiResults[1].documentId == 1);

    // --------------------------------------------
    // Test 7: Boolean filtering + TF-IDF ranking
    // --------------------------------------------

    InvertedIndex booleanIndex;

    // Document 1
    // machine appears twice
    // learning appears once
    booleanIndex.add("machine", 1);
    booleanIndex.add("machine", 1);
    booleanIndex.add("learning", 1);

    // Document 2
    // machine appears once
    // learning appears once
    booleanIndex.add("machine", 2);
    booleanIndex.add("learning", 2);

    // Document 3
    // machine only
    booleanIndex.add("machine", 3);

    QueryProcessor booleanQueryProcessor(
        booleanIndex,
        textProcessor
    );

    Ranker booleanRanker(
        booleanIndex,
        textProcessor,
        booleanQueryProcessor
    );

    auto booleanResults =
        booleanRanker.rank(
            "machine AND learning"
        );

    // Only documents containing BOTH terms
    // should be candidates.
    assert(booleanResults.size() == 2);

    // Document 1 has higher machine TF,
    // so it should rank above Document 2.
    assert(booleanResults[0].documentId == 1);
    assert(booleanResults[1].documentId == 2);

        // --------------------------------------------
    // Test 8: Boolean OR + TF-IDF ranking
    // --------------------------------------------

    InvertedIndex orIndex;

    // Document 1
    // machine × 3
    orIndex.add("machine", 1);
    orIndex.add("machine", 1);
    orIndex.add("machine", 1);

    // Document 2
    // learning × 2
    orIndex.add("learning", 2);
    orIndex.add("learning", 2);

    // Document 3
    // machine × 1
    // learning × 1
    orIndex.add("machine", 3);
    orIndex.add("learning", 3);

    // Document 4
    // unrelated
    orIndex.add("computer", 4);

    QueryProcessor orQueryProcessor(
        orIndex,
        textProcessor
    );

    Ranker orRanker(
        orIndex,
        textProcessor,
        orQueryProcessor
    );

    auto orResults =
        orRanker.rank(
            "machine OR learning"
        );

    // Documents 1, 2 and 3 satisfy the OR query.
    assert(orResults.size() == 3);

    // machine appears in documents 1 and 3.
    // learning appears in documents 2 and 3.
    //
    // Both terms therefore contribute to the score
    // of Document 3.
    assert(orResults[0].documentId == 1);
    assert(orResults[1].documentId == 2);
    assert(orResults[2].documentId == 3);

        // --------------------------------------------
    // Test 9: Boolean NOT + TF-IDF ranking
    // --------------------------------------------

    InvertedIndex notIndex;

    // Document 1
    notIndex.add("machine", 1);
    notIndex.add("learning", 1);

    // Document 2
    notIndex.add("machine", 2);
    notIndex.add("computer", 2);

    // Document 3
    notIndex.add("learning", 3);

    QueryProcessor notQueryProcessor(
        notIndex,
        textProcessor
    );

    Ranker notRanker(
        notIndex,
        textProcessor,
        notQueryProcessor
    );

    auto notResults =
        notRanker.rank(
            "machine NOT learning"
        );

    // Only Document 2 contains machine
    // without learning.
    assert(notResults.size() == 1);
    assert(notResults[0].documentId == 2);

    std::cout
        << "All TF-IDF Ranker tests passed!\n";

    return 0;
}