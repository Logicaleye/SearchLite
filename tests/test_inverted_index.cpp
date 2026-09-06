#include <iostream>
#include <vector>
#include <cassert>

#include "InvertedIndex.h"

using namespace std;

void printResults(const vector<int>& results) {
    for (int id : results) {
        cout << id << " ";
    }

    cout << '\n';
}

int main() {
    InvertedIndex index;

    // Add documents
    index.add("machine", 1);
    index.add("learning", 1);

    index.add("machine", 2);
    index.add("learning", 2);

    index.add("computer", 3);

    // Test 1: Search existing term
    auto machineResults = index.search("machine");

    cout << "machine -> ";
    printResults(machineResults);

    assert(machineResults.size() == 2);
    assert(machineResults[0] == 1);
    assert(machineResults[1] == 2);

    // Test 2: Search another existing term
    auto learningResults = index.search("learning");

    cout << "learning -> ";
    printResults(learningResults);

    assert(learningResults.size() == 2);

    // Test 3: Search unknown term
    auto unknownResults = index.search("quantum");

    cout << "quantum -> ";
    printResults(unknownResults);

    assert(unknownResults.empty());

    // Test 4: contains()
    assert(index.contains("machine"));
    assert(!index.contains("quantum"));

    // Test 5: number of unique terms
    assert(index.size() == 3);

    // Test 6: duplicate document
    index.add("machine", 2);

    auto duplicateResults = index.search("machine");

    assert(duplicateResults.size() == 2);

    cout << "\nAll tests passed!\n";

    return 0;
}