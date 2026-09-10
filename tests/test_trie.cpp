#include "Trie.h"

#include <cassert>
#include <iostream>

int main() {

    Trie trie;

    // Insert words
    trie.insert("machine");
    trie.insert("machines");
    trie.insert("map");
    trie.insert("mapping");
    trie.insert("computer");
    trie.insert("computerized");

    // Exact word searches
    assert(trie.search("machine"));
    assert(trie.search("machines"));
    assert(trie.search("map"));
    assert(trie.search("mapping"));
    assert(trie.search("computer"));
    assert(trie.search("computerized"));
    
    // Prefixes should not be considered complete words
    assert(!trie.search("mach"));
    assert(!trie.search("comput"));

    // Unknown words
    assert(!trie.search("quantum"));
    assert(!trie.search("apple"));

    // Prefix searches
    assert(trie.startsWith("mach"));
    assert(trie.startsWith("machine"));
    assert(trie.startsWith("map"));
    assert(trie.startsWith("comp"));
    assert(trie.startsWith("computer"));

    // Unknown prefixes
    assert(!trie.startsWith("xyz"));
    assert(!trie.startsWith("quant"));

    // Case handling
    assert(trie.search("Machine"));
    assert(trie.startsWith("Mach"));

        // Autocomplete tests

    auto machineSuggestions =
        trie.suggest("mach");

    assert(machineSuggestions.size() == 2);
    assert(machineSuggestions[0] == "machine");
    assert(machineSuggestions[1] == "machines");


    auto computerSuggestions =
        trie.suggest("comp");

    assert(computerSuggestions.size() == 2);
    assert(computerSuggestions[0] == "computer");
    assert(computerSuggestions[1] == "computerized");


        auto mapSuggestions =
        trie.suggest("map");

    assert(mapSuggestions.size() == 2);
    assert(mapSuggestions[0] == "map");
    assert(mapSuggestions[1] == "mapping");


    auto unknownSuggestions =
        trie.suggest("quant");

    assert(unknownSuggestions.empty());


    auto caseSuggestions =
        trie.suggest("MACH");

    assert(caseSuggestions.size() == 2);
    assert(caseSuggestions[0] == "machine");
    assert(caseSuggestions[1] == "machines");

    std::cout << "All Trie tests passed!\n";

    return 0;
}