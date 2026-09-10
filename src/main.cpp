#include "DocumentLoader.h"
#include "InvertedIndex.h"
#include "QueryProcessor.h"
#include "TextProcessor.h"
#include "Ranker.h"
#include "Trie.h"
#include <iostream>
#include <string>

int main() {

    DocumentLoader loader;
    TextProcessor textProcessor;
    InvertedIndex index;
    Trie trie;

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
        trie.insert(token);
        }
    }

    std::cout << "Unique terms indexed: "
              << index.size()
              << "\n\n";

    // Create query processor
    QueryProcessor queryProcessor(index, textProcessor);
    Ranker ranker(index, textProcessor, queryProcessor);
    // Search loop
    while (true) {

        std::cout << "Search> ";

        std::string query;
        std::getline(std::cin, query);

        if (query == "exit") {
            break;
        }
        if (query.rfind("suggest ", 0) == 0) {

    std::string prefix =
        query.substr(8);

    auto suggestions =
        trie.suggest(prefix);

    if (suggestions.empty()) {
        std::cout
            << "No suggestions found.\n\n";
        continue;
    }

    std::cout << "Suggestions:\n";

    for (const auto& suggestion : suggestions) {
        std::cout << suggestion << '\n';
    }

    std::cout << '\n';

    continue;
}

        auto results =
            ranker.rank(query);

        if (results.empty()) {
            std::cout << "No documents found.\n\n";
            continue;
        }

        std::cout << "Documents found: ";

        for (const auto& result : results) {
    std::cout
        << "Document ID: "
        << result.documentId
        << " | Score: "
        << result.score
        << '\n';
}

        std::cout << "\n\n";
    }

    return 0;
}