#include "IndexPersistence.h"
#include "IndexMetadata.h"

#include <fstream>
#include <vector>
#include <unordered_map>

bool IndexPersistence::save(
    const InvertedIndex& index,
    const std::string& filename
) {
    std::ofstream output(
        filename,
        std::ios::binary
    );

    if (!output) {
        return false;
    }

    // --------------------------------------------------
    // Save document IDs
    // --------------------------------------------------

    const auto documents =
        index.getAllDocuments();

    output << documents.size() << '\n';

    for (int documentId : documents) {
        output << documentId << '\n';
    }

    // --------------------------------------------------
    // Save terms
    // --------------------------------------------------

    const auto terms =
        index.getTerms();

    output << terms.size() << '\n';

    for (const auto& term : terms) {

        output << term << '\n';

        // --------------------------------------------------
        // Save term frequencies
        // --------------------------------------------------

        const auto& postings =
            index.getPostings(term);

        output << postings.size() << '\n';

        for (const auto& [documentId, frequency] :
             postings) {

            output
                << documentId
                << ' '
                << frequency
                << '\n';
        }

        // --------------------------------------------------
        // Save positional information
        // --------------------------------------------------

        const auto& positionalPostings =
            index.getPositions(term);

        output
            << positionalPostings.size()
            << '\n';

        for (
            const auto& [documentId, positions] :
            positionalPostings
        ) {

            output
                << documentId
                << ' '
                << positions.size();

            for (int position : positions) {
                output
                    << ' '
                    << position;
            }

            output << '\n';
        }
    }

    return output.good();
}


bool IndexPersistence::load(
    InvertedIndex& index,
    const std::string& filename
) {
    std::ifstream input(
        filename,
        std::ios::binary
    );

    if (!input) {
        return false;
    }

    // --------------------------------------------------
    // Load into temporary index first
    // --------------------------------------------------
    // This preserves atomic loading:
    // if anything fails, the existing index
    // remains unchanged.

    InvertedIndex temporaryIndex;

    // --------------------------------------------------
    // Load document IDs
    // --------------------------------------------------

    std::size_t documentCount;

    if (!(input >> documentCount)) {
        return false;
    }

    std::vector<int> documents(
        documentCount
    );

    for (
        std::size_t i = 0;
        i < documentCount;
        ++i
    ) {

        if (!(input >> documents[i])) {
            return false;
        }
    }

    // --------------------------------------------------
    // Load terms
    // --------------------------------------------------

    std::size_t termCount;

    if (!(input >> termCount)) {
        return false;
    }

    for (
        std::size_t i = 0;
        i < termCount;
        ++i
    ) {

        std::string term;

        if (!(input >> term)) {
            return false;
        }

        // --------------------------------------------------
        // Load term frequencies
        // --------------------------------------------------

        std::size_t postingCount;

        if (!(input >> postingCount)) {
            return false;
        }

        std::unordered_map<int, int> frequencies;

        for (
            std::size_t j = 0;
            j < postingCount;
            ++j
        ) {

            int documentId;
            int frequency;

            if (
                !(input >>
                  documentId >>
                  frequency)
            ) {
                return false;
            }

            frequencies[documentId] =
                frequency;
        }

        // --------------------------------------------------
        // Load positional information
        // --------------------------------------------------

        std::size_t positionalCount;

        if (!(input >> positionalCount)) {
            return false;
        }

        std::unordered_map<
            int,
            std::vector<int>
        > positionalData;

        for (
            std::size_t j = 0;
            j < positionalCount;
            ++j
        ) {

            int documentId;
            std::size_t positionCount;

            if (
                !(input >>
                  documentId >>
                  positionCount)
            ) {
                return false;
            }

            std::vector<int> positions(
                positionCount
            );

            for (
                std::size_t k = 0;
                k < positionCount;
                ++k
            ) {

                if (!(input >> positions[k])) {
                    return false;
                }
            }

            positionalData[documentId] =
                positions;
        }

        // --------------------------------------------------
        // Restore exact index state
        // --------------------------------------------------

        for (
            const auto& [documentId, frequency] :
            frequencies
        ) {

            auto positionIt =
                positionalData.find(documentId);

            if (
                positionIt !=
                positionalData.end()
            ) {

                temporaryIndex.restore(
                    term,
                    documentId,
                    frequency,
                    positionIt->second
                );

            } else {

                temporaryIndex.restore(
                    term,
                    documentId,
                    frequency,
                    {}
                );
            }
        }
    }

    // --------------------------------------------------
    // Atomically replace existing index
    // --------------------------------------------------

    index.replaceWith(
        temporaryIndex
    );

    return true;
}


// ======================================================
// Metadata Persistence
// ======================================================

bool IndexPersistence::saveMetadata(
    const IndexMetadata& metadata,
    const std::string& filename
) {
    std::ofstream output(
        filename,
        std::ios::trunc
    );

    if (!output) {
        return false;
    }

    const auto& documents =
        metadata.getDocuments();

    output << documents.size() << '\n';

    for (
        const auto& [path, timestamp] :
        documents
    ) {

        output
            << path
            << '\t'
            << timestamp
            << '\n';
    }

    return output.good();
}


bool IndexPersistence::loadMetadata(
    IndexMetadata& metadata,
    const std::string& filename
) {
    std::ifstream input(filename);

    if (!input) {
        return false;
    }

    std::size_t documentCount;

    if (!(input >> documentCount)) {
        return false;
    }

    std::string path;
    long long timestamp;

    for (
        std::size_t i = 0;
        i < documentCount;
        ++i
    ) {

        if (!(input >> path >> timestamp)) {
            return false;
        }

        metadata.addDocument(
            path,
            timestamp
        );
    }

    return true;
}