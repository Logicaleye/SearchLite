#include "DocumentLoader.h"

#include <cassert>
#include <iostream>

int main() {

    DocumentLoader loader;

    auto documents =
        loader.loadDocuments("data");

    // We created three .txt files.
    assert(documents.size() == 3);

    for (const auto& document : documents) {

        assert(document.id > 0);
        assert(!document.path.empty());

        std::string content =
            loader.readDocument(document);

        assert(!content.empty());
    }

    std::cout << "All DocumentLoader tests passed!\n";

    return 0;
}