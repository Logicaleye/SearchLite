#include "IndexPersistence.h"
#include "InvertedIndex.h"
#include "TextProcessor.h"
#include "QueryProcessor.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>

int main() {

    const std::string filename =
        "test_index.dat";

    // --------------------------------------------------
    // Build original index
    // --------------------------------------------------

    InvertedIndex original;

    // Document 1:
    // machine learning algorithms
    original.add("machine", 1, 0);
    original.add("learning", 1, 1);
    original.add("algorithms", 1, 2);

    // Document 2:
    // machine algorithms
    original.add("machine", 2, 0);
    original.add("algorithms", 2, 1);

    // Document 3:
    // computer vision
    original.add("computer", 3, 0);
    original.add("vision", 3, 1);

    // --------------------------------------------------
    // Test 1: Save
    // --------------------------------------------------

    bool saved =
        IndexPersistence::save(
            original,
            filename
        );

    assert(saved);

    // --------------------------------------------------
    // Test 2: Load
    // --------------------------------------------------

    InvertedIndex restored;

    bool loaded =
        IndexPersistence::load(
            restored,
            filename
        );

    assert(loaded);

    // --------------------------------------------------
    // Test 3: Document IDs
    // --------------------------------------------------

    auto originalDocuments =
        original.getAllDocuments();

    auto restoredDocuments =
        restored.getAllDocuments();

    assert(
        originalDocuments ==
        restoredDocuments
    );

    // --------------------------------------------------
    // Test 4: Terms
    // --------------------------------------------------

    auto originalTerms =
        original.getTerms();

    auto restoredTerms =
        restored.getTerms();

    assert(
        originalTerms ==
        restoredTerms
    );

    // --------------------------------------------------
    // Test 5: Postings
    // --------------------------------------------------

    for (const auto& term :
         originalTerms) {

        auto originalResults =
            original.search(term);

        auto restoredResults =
            restored.search(term);

        assert(
            originalResults ==
            restoredResults
        );
    }

    // --------------------------------------------------
    // Test 6: Term frequencies
    // --------------------------------------------------

    {
        const auto& originalPostings =
            original.getPostings("machine");

        const auto& restoredPostings =
            restored.getPostings("machine");

        assert(
            originalPostings ==
            restoredPostings
        );
    }

    // --------------------------------------------------
    // Test 7: Positional information
    // --------------------------------------------------

    {
        const auto& originalPositions =
            original.getPositions("machine");

        const auto& restoredPositions =
            restored.getPositions("machine");

        assert(
            originalPositions ==
            restoredPositions
        );
    }

    {
        const auto& originalPositions =
            original.getPositions("learning");

        const auto& restoredPositions =
            restored.getPositions("learning");

        assert(
            originalPositions ==
            restoredPositions
        );
    }

    // --------------------------------------------------
    // Test 8: Phrase search after loading
    // --------------------------------------------------

    auto originalPhrase =
        original.searchPhrase(
            {
                "machine",
                "learning"
            }
        );

    auto restoredPhrase =
        restored.searchPhrase(
            {
                "machine",
                "learning"
            }
        );

    assert(
        originalPhrase ==
        restoredPhrase
    );

    assert(
        (restoredPhrase ==
         std::vector<int>{1})
    );

    // --------------------------------------------------
    // Test 9: QueryProcessor after loading
    // --------------------------------------------------

    TextProcessor textProcessor;

    QueryProcessor originalProcessor(
        original,
        textProcessor
    );

    QueryProcessor restoredProcessor(
        restored,
        textProcessor
    );

    auto originalQuery =
        originalProcessor.search(
            "machine learning"
        );

    auto restoredQuery =
        restoredProcessor.search(
            "machine learning"
        );

    assert(
        originalQuery ==
        restoredQuery
    );

    assert(
        (restoredQuery ==
         std::vector<int>{1})
    );

    // --------------------------------------------------
    // Test 10: Boolean search after loading
    // --------------------------------------------------

    auto originalBoolean =
        originalProcessor.searchBoolean(
            "machine AND algorithms"
        );

    auto restoredBoolean =
        restoredProcessor.searchBoolean(
            "machine AND algorithms"
        );

    assert(
        originalBoolean ==
        restoredBoolean
    );

    assert(
        (restoredBoolean ==
         std::vector<int>{1, 2})
    );

    // --------------------------------------------------
    // Test 11: Missing file
    // --------------------------------------------------

    InvertedIndex emptyIndex;

    bool missing =
        IndexPersistence::load(
            emptyIndex,
            "file_that_does_not_exist.dat"
        );

    assert(!missing);

    // --------------------------------------------------
// Test 12: Corrupted index file
// --------------------------------------------------

{
    const std::string corruptedFile =
        "corrupted_index.dat";

    {
        std::ofstream output(
            corruptedFile,
            std::ios::binary
        );

        output << "this is not a valid index";
    }

    InvertedIndex corruptedIndex;

    bool result =
        IndexPersistence::load(
            corruptedIndex,
            corruptedFile
        );

    assert(!result);

    std::remove(
        corruptedFile.c_str()
    );
}

// --------------------------------------------------
// Test 13: Truncated index file
// --------------------------------------------------

{
    const std::string truncatedFile =
        "truncated_index.dat";

    {
        std::ofstream output(
            truncatedFile,
            std::ios::binary
        );

        // Claims that there are 3 documents,
        // but provides only one.
        output << "3\n";
        output << "1\n";
    }

    InvertedIndex truncatedIndex;

    bool result =
        IndexPersistence::load(
            truncatedIndex,
            truncatedFile
        );

    assert(!result);

    std::remove(
        truncatedFile.c_str()
    );
}

// --------------------------------------------------
// Test 14: Failed load does not modify existing index
// --------------------------------------------------

{
    const std::string corruptedFile =
        "atomicity_test.dat";

    {
        std::ofstream output(
            corruptedFile,
            std::ios::binary
        );

        output << "1\n";
        output << "999\n";

        // Deliberately incomplete file.
    }

    InvertedIndex existingIndex;

    existingIndex.add(
        "existing",
        1,
        0
    );

    bool result =
        IndexPersistence::load(
            existingIndex,
            corruptedFile
        );

    assert(!result);

    // Existing index must remain unchanged.
    assert(
        (existingIndex.search("existing") ==
         std::vector<int>{1})
    );

    assert(
        existingIndex.search("999").empty()
    );

    std::remove(
        corruptedFile.c_str()
    );
}

    // --------------------------------------------------
    // Cleanup
    // --------------------------------------------------

    std::remove(
        filename.c_str()
    );

    std::cout
        << "All IndexPersistence tests passed!"
        << std::endl;

    return 0;
}