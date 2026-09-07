#include "DocumentLoader.h"
#include "InvertedIndex.h"
#include "TextProcessor.h"

#include <iostream>

int main() {

    DocumentLoader loader;
    TextProcessor processor;
    InvertedIndex index;

    // Step 1: Load documents
    auto documents = loader.loadDocuments("data");

    std::cout << "Documents loaded: "
              << documents.size() << "\n\n";

    // Step 2: Process each document
    for (const auto& document : documents) {

        std::string content =
            loader.readDocument(document);

        auto tokens =
            processor.process(content);

        // Step 3: Add terms to inverted index
        for (const auto& token : tokens) {
            index.add(token, document.id);
        }

        std::cout << "Indexed Document "
                  << document.id
                  << ": "
                  << document.path
                  << "\n";
    }

    // Step 4: Display index statistics
    std::cout << "\nUnique terms indexed: "
              << index.size()
              << "\n";

    // Step 5: Test a search
    std::string query = "machine";

    auto results = index.search(query);

    std::cout << "\nSearch: "
              << query
              << "\n";

    std::cout << "Documents: ";

    for (int documentId : results) {
        std::cout << documentId << " ";
    }

    std::cout << "\n";

    return 0;
}