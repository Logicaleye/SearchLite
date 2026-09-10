#include "TextProcessor.h"
#include "Trie.h"

#include <cassert>
#include <iostream>

int main() {

    TextProcessor textProcessor;
    Trie trie;

    std::string document =
        "Machine learning algorithms process data. "
        "Machine learning is useful for computer vision.";

    // Process the document exactly like SearchLite does.
    auto tokens =
        textProcessor.process(document);

    // Add processed tokens to the Trie.
    for (const auto& token : tokens) {
        trie.insert(token);
    }

    // Exact word searches
    assert(trie.search("machine"));
    assert(trie.search("learning"));
    assert(trie.search("algorithms"));
    assert(trie.search("data"));
    assert(trie.search("computer"));
    assert(trie.search("vision"));

    // Prefix searches
    assert(trie.startsWith("mach"));
    assert(trie.startsWith("learn"));
    assert(trie.startsWith("comp"));
    assert(trie.startsWith("vis"));

    // Autocomplete
    auto machineSuggestions =
        trie.suggest("mach");

    assert(!machineSuggestions.empty());
    assert(machineSuggestions[0] == "machine");

    auto computerSuggestions =
        trie.suggest("comp");

    assert(!computerSuggestions.empty());
    assert(computerSuggestions[0] == "computer");

    // Unknown prefix
    auto unknownSuggestions =
        trie.suggest("quant");

    assert(unknownSuggestions.empty());

    std::cout
        << "All Trie integration tests passed!\n";

    return 0;
}