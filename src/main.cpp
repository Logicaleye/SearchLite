#include "DocumentLoader.h"
#include "InvertedIndex.h"
#include "QueryProcessor.h"
#include "TextProcessor.h"
#include "Ranker.h"
#include "Trie.h"
#include "BooleanParser.h"
#include "IndexPersistence.h"

#include <iostream>
#include <string>

int main() {

    const std::string indexFile =
        "data/index.dat";

    DocumentLoader loader;
    TextProcessor textProcessor;
    InvertedIndex index;
    Trie trie;

    // --------------------------------------------------
    // Load persisted index or build a new index
    // --------------------------------------------------

    bool indexLoaded =
        IndexPersistence::load(
            index,
            indexFile
        );

    if (indexLoaded) {

        std::cout
            << "Loaded persisted index."
            << std::endl;

        // Rebuild Trie from persisted terms
        // because Trie is not currently persisted.
        for (const auto& term : index.getTerms()) {

            trie.insert(term);
        }

    } else {

        std::cout
            << "Building index..."
            << std::endl;

        // --------------------------------------------------
        // Load documents
        // --------------------------------------------------

        auto documents =
            loader.loadDocuments("data");

        std::cout
            << "Documents loaded: "
            << documents.size()
            << "\n\n";

        // --------------------------------------------------
        // Build index
        // --------------------------------------------------

        for (const auto& document : documents) {

            std::string content =
                loader.readDocument(document);

            auto tokens =
                textProcessor.process(content);

            for (
                std::size_t position = 0;
                position < tokens.size();
                ++position
            ) {

                const std::string& token =
                    tokens[position];

                // Add term + document + position
                index.add(
                    token,
                    document.id,
                    static_cast<int>(position)
                );

                // Add term to autocomplete Trie
                trie.insert(token);
            }
        }

        // --------------------------------------------------
        // Save newly built index
        // --------------------------------------------------

        bool saved =
            IndexPersistence::save(
                index,
                indexFile
            );

        if (saved) {

            std::cout
                << "Index saved successfully."
                << std::endl;

        } else {

            std::cerr
                << "Warning: Failed to save index."
                << std::endl;
        }
    }

    // --------------------------------------------------
    // Index information
    // --------------------------------------------------

    std::cout
        << "Unique terms indexed: "
        << index.size()
        << "\n\n";

    // --------------------------------------------------
    // Create query processor and ranker
    // --------------------------------------------------

    QueryProcessor queryProcessor(
        index,
        textProcessor
    );

    Ranker ranker(
        index,
        textProcessor,
        queryProcessor
    );

    // --------------------------------------------------
    // Search loop
    // --------------------------------------------------

    while (true) {

        std::cout << "Search> ";

        std::string query;
        std::getline(
            std::cin,
            query
        );

        // --------------------------------------------------
        // Exit
        // --------------------------------------------------

        if (query == "exit") {
            break;
        }

        // --------------------------------------------------
        // Autocomplete
        // Example:
        // suggest mach
        // --------------------------------------------------

        if (
            query.rfind("suggest ", 0) == 0
        ) {

            std::string prefix =
                query.substr(8);

            auto suggestions =
                trie.suggest(prefix);

            if (suggestions.empty()) {

                std::cout
                    << "No suggestions found.\n\n";

                continue;
            }

            std::cout
                << "Suggestions:\n";

            for (
                const auto& suggestion :
                suggestions
            ) {

                std::cout
                    << suggestion
                    << '\n';
            }

            std::cout << '\n';

            continue;
        }

        // --------------------------------------------------
        // Boolean search
        // --------------------------------------------------

        if (
            queryProcessor.isBooleanQuery(
                query
            )
        ) {

            auto booleanResults =
                queryProcessor.searchBoolean(
                    query
                );

            if (booleanResults.empty()) {

                std::cout
                    << "No documents found.\n\n";

                continue;
            }

            std::cout
                << "Boolean matches:\n";

            for (
                int documentId :
                booleanResults
            ) {

                std::cout
                    << "Document ID: "
                    << documentId
                    << '\n';
            }

            std::cout << '\n';

            continue;
        }

        // --------------------------------------------------
        // Normal / phrase / ranked search
        // --------------------------------------------------

        auto results =
            ranker.rank(
                query,
                10
            );

        if (results.empty()) {

            std::cout
                << "No documents found.\n\n";

            continue;
        }

        std::cout
            << "Documents found:\n";

        for (
            const auto& result :
            results
        ) {

            std::cout
                << "Document ID: "
                << result.documentId
                << " | Score: "
                << result.score
                << '\n';
        }

        std::cout << '\n';
    }

    return 0;
}