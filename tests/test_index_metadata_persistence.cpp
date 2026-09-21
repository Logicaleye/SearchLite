#include "IndexMetadata.h"
#include "IndexPersistence.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

int main() {

    const std::string metadataFile =
        "test_index.meta";

    // --------------------------------------------------
    // Cleanup previous test file
    // --------------------------------------------------

    if (fs::exists(metadataFile)) {
        fs::remove(metadataFile);
    }

    // --------------------------------------------------
    // Create original metadata
    // --------------------------------------------------

    IndexMetadata original;

    original.addDocument(
        "data/doc1.txt",
        1000
    );

    original.addDocument(
        "data/doc2.txt",
        2000
    );

    original.addDocument(
        "data/doc3.txt",
        3000
    );

    assert(original.size() == 3);

    // --------------------------------------------------
    // Save metadata
    // --------------------------------------------------

    bool saved =
        IndexPersistence::saveMetadata(
            original,
            metadataFile
        );

    assert(saved);
    assert(fs::exists(metadataFile));

    // --------------------------------------------------
    // Load metadata
    // --------------------------------------------------

    IndexMetadata loaded;

    bool loadedSuccessfully =
        IndexPersistence::loadMetadata(
            loaded,
            metadataFile
        );

    assert(loadedSuccessfully);

    // --------------------------------------------------
    // Verify document count
    // --------------------------------------------------

    assert(loaded.size() == 3);

    // --------------------------------------------------
    // Verify timestamps
    // --------------------------------------------------

    assert(
        loaded.contains("data/doc1.txt")
    );

    assert(
        loaded.contains("data/doc2.txt")
    );

    assert(
        loaded.contains("data/doc3.txt")
    );

    assert(
        loaded.getTimestamp("data/doc1.txt")
        == 1000
    );

    assert(
        loaded.getTimestamp("data/doc2.txt")
        == 2000
    );

    assert(
        loaded.getTimestamp("data/doc3.txt")
        == 3000
    );

    // --------------------------------------------------
    // Test missing metadata file
    // --------------------------------------------------

    IndexMetadata missingMetadata;

    bool missingResult =
        IndexPersistence::loadMetadata(
            missingMetadata,
            "does_not_exist.meta"
        );

    assert(!missingResult);

    // --------------------------------------------------
    // Test corrupted metadata
    // --------------------------------------------------

    {
        std::ofstream corrupted(
            metadataFile,
            std::ios::trunc
        );

        corrupted
            << "3\n"
            << "data/doc1.txt 1000\n"
            << "data/doc2.txt 2000\n";
    }

    IndexMetadata corruptedMetadata;

    bool corruptedResult =
        IndexPersistence::loadMetadata(
            corruptedMetadata,
            metadataFile
        );

    assert(!corruptedResult);

    // --------------------------------------------------
    // Cleanup
    // --------------------------------------------------

    fs::remove(metadataFile);

    std::cout
        << "All IndexMetadata persistence tests passed!"
        << std::endl;

    return 0;
}