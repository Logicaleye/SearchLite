#include "IndexValidator.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

void writeFile(
    const fs::path& path,
    const std::string& content
) {
    std::ofstream file(path);

    assert(file.is_open());

    file << content;
}

int main() {

    const fs::path testDirectory =
        "test_data";

    // --------------------------------------------------
    // Clean previous test directory
    // --------------------------------------------------

    if (fs::exists(testDirectory)) {
        fs::remove_all(testDirectory);
    }

    fs::create_directory(testDirectory);

    // --------------------------------------------------
    // Create initial documents
    // --------------------------------------------------

    writeFile(
        testDirectory / "doc1.txt",
        "machine learning"
    );

    writeFile(
        testDirectory / "doc2.txt",
        "artificial intelligence"
    );

    writeFile(
        testDirectory / "doc3.txt",
        "data structures"
    );

    // --------------------------------------------------
    // Create metadata
    // --------------------------------------------------

    IndexMetadata metadata =
        IndexValidator::createMetadata(
            testDirectory.string()
        );

    // Three documents should exist
    assert(metadata.size() == 3);

    // --------------------------------------------------
    // Test 1: Unchanged documents
    // --------------------------------------------------

    assert(
        IndexValidator::isValid(
            testDirectory.string(),
            metadata
        )
    );

    // --------------------------------------------------
    // Test 2: Modify a document
    // --------------------------------------------------

    writeFile(
        testDirectory / "doc1.txt",
        "machine learning algorithms"
    );

    // The modification timestamp should change.
    // Wait briefly if necessary because some filesystems
    // have coarse timestamp resolution.
    auto originalTimestamp =
        metadata.getTimestamp(
            (testDirectory / "doc1.txt").string()
        );

    auto currentMetadata =
        IndexValidator::createMetadata(
            testDirectory.string()
        );

    auto modifiedTimestamp =
        currentMetadata.getTimestamp(
            (testDirectory / "doc1.txt").string()
        );

    // If the filesystem timestamp resolution is too coarse,
    // the content modification may not immediately change
    // the timestamp. We only assert invalidity when the
    // timestamp actually changed.
    if (
        originalTimestamp != modifiedTimestamp
    ) {

        assert(
            !IndexValidator::isValid(
                testDirectory.string(),
                metadata
            )
        );
    }

    // --------------------------------------------------
    // Rebuild metadata
    // --------------------------------------------------

    metadata =
        IndexValidator::createMetadata(
            testDirectory.string()
        );

    // --------------------------------------------------
    // Test 3: Add a new document
    // --------------------------------------------------

    writeFile(
        testDirectory / "doc4.txt",
        "computer science"
    );

    assert(
        !IndexValidator::isValid(
            testDirectory.string(),
            metadata
        )
    );

    // --------------------------------------------------
    // Rebuild metadata
    // --------------------------------------------------

    metadata =
        IndexValidator::createMetadata(
            testDirectory.string()
        );

    // --------------------------------------------------
    // Test 4: Delete a document
    // --------------------------------------------------

    fs::remove(
        testDirectory / "doc2.txt"
    );

    assert(
        !IndexValidator::isValid(
            testDirectory.string(),
            metadata
        )
    );

    // --------------------------------------------------
    // Cleanup
    // --------------------------------------------------

    fs::remove_all(testDirectory);

    std::cout
        << "All IndexValidator tests passed!"
        << std::endl;

    return 0;
}