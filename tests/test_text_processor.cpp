#include <cassert>
#include <iostream>
#include <vector>

#include "TextProcessor.h"

using namespace std;

int main() {

    TextProcessor processor;

    // Test 1: Normalisation
    string text = "Hello, WORLD!";

    string normalized = processor.normalize(text);

    assert(normalized == "hello  world ");

    // Test 2: Tokenisation
    auto tokens = processor.tokenize("hello world search");

    assert(tokens.size() == 3);
    assert(tokens[0] == "hello");
    assert(tokens[1] == "world");
    assert(tokens[2] == "search");

    // Test 3: Stop-word removal
    auto processed = processor.process(
        "Machine learning is a branch of artificial intelligence"
    );

    assert(processed.size() == 5);

    assert(processed[0] == "machine");
    assert(processed[1] == "learning");
    assert(processed[2] == "branch");
    assert(processed[3] == "artificial");
    assert(processed[4] == "intelligence");

    // Test 4: Empty input
    auto empty = processor.process("");

    assert(empty.empty());

    cout << "All TextProcessor tests passed!\n";

    return 0;
}