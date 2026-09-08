#include "DocumentLoader.h"
#include "InvertedIndex.h"
#include "QueryProcessor.h"
#include "TextProcessor.h"

#include <iostream>
#include <string>

int main() {

    DocumentLoader loader;
    TextProcessor textProcessor;
    InvertedIndex index;

    // Load documents
    auto documents =
        loader.loadDocuments("data");

    std::cout << "Documents loaded: "
              << documents.size()
              << "\n\n";

    // Build index
    for (const auto& document : documents) {

        std::string content =
            loader.readDocument(document);

        auto tokens =
            textProcessor.process(content);

        for (const auto& token : tokens) {
            index.add(token, document.id);
        }
    }

    std::cout << "Unique terms indexed: "
              << index.size()
              << "\n\n";

    // Create query processor
    QueryProcessor queryProcessor(
        index,
        textProcessor
    );

    // Search loop
    while (true) {

        std::cout << "Search> ";

        std::string query;
        std::getline(std::cin, query);

        if (query == "exit") {
            break;
        }

        auto results =
            queryProcessor.search(query);

        if (results.empty()) {
            std::cout << "No documents found.\n\n";
            continue;
        }

        std::cout << "Documents found: ";

        for (int documentId : results) {
            std::cout << documentId << " ";
        }

        std::cout << "\n\n";
    }

    return 0;
}