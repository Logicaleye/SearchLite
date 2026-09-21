#include "IndexMetadata.h"

#include <cassert>
#include <iostream>

int main() {

    IndexMetadata metadata;

    // --------------------------------------------------
    // Test 1: Empty metadata
    // --------------------------------------------------

    assert(metadata.size() == 0);

    // --------------------------------------------------
    // Test 2: Add first document
    // --------------------------------------------------

    metadata.addDocument(
        "data/doc1.txt",
        1000
    );

    assert(metadata.size() == 1);
    assert(metadata.contains("data/doc1.txt"));
    assert(
        metadata.getTimestamp("data/doc1.txt")
        == 1000
    );

    // --------------------------------------------------
    // Test 3: Add second document
    // --------------------------------------------------

    metadata.addDocument(
        "data/doc2.txt",
        2000
    );

    assert(metadata.size() == 2);
    assert(metadata.contains("data/doc2.txt"));
    assert(
        metadata.getTimestamp("data/doc2.txt")
        == 2000
    );

    // --------------------------------------------------
    // Test 4: Non-existing document
    // --------------------------------------------------

    assert(
        !metadata.contains("data/doc3.txt")
    );

    assert(
        metadata.getTimestamp("data/doc3.txt")
        == -1
    );

    // --------------------------------------------------
    // Test 5: Updating existing document
    // --------------------------------------------------

    metadata.addDocument(
        "data/doc1.txt",
        5000
    );

    assert(metadata.size() == 2);

    assert(
        metadata.getTimestamp("data/doc1.txt")
        == 5000
    );

    // --------------------------------------------------
    // Test 6: Verify stored documents
    // --------------------------------------------------

    const auto& documents =
        metadata.getDocuments();

    assert(documents.size() == 2);
    assert(documents.at("data/doc1.txt") == 5000);
    assert(documents.at("data/doc2.txt") == 2000);

    std::cout
        << "All IndexMetadata tests passed!"
        << std::endl;

    return 0;
}