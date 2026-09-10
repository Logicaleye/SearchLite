#include "Trie.h"

#include <cctype>
#include <stdexcept>

TrieNode::TrieNode()
    : isEndOfWord(false) {
}

Trie::Trie()
    : root(std::make_unique<TrieNode>()) {
}
//insert fun
void Trie::insert(const std::string& word) {

    if (word.empty()) {
        return;
    }

    TrieNode* current = root.get();

    for (unsigned char ch : word) {

        if (!std::isalpha(ch)) {
            continue;
        }

        char lowerChar =
            static_cast<char>(std::tolower(ch));

        int index =
            lowerChar - 'a';

        if (index < 0 || index >= 26) {
            continue;
        }

        if (!current->children[index]) {
            current->children[index] =
                std::make_unique<TrieNode>();
        }

        current =
            current->children[index].get();
    }

    current->isEndOfWord = true;
}
// search fun
bool Trie::search(
    const std::string& word
) const {

    if (word.empty()) {
        return false;
    }

    const TrieNode* current = root.get();

    for (unsigned char ch : word) {

        if (!std::isalpha(ch)) {
            return false;
        }

        char lowerChar =
            static_cast<char>(std::tolower(ch));

        int index =
            lowerChar - 'a';

        if (index < 0 || index >= 26) {
            return false;
        }

        if (!current->children[index]) {
            return false;
        }

        current =
            current->children[index].get();
    }

    return current->isEndOfWord;
}


bool Trie::startsWith(
    const std::string& prefix
) const {

    if (prefix.empty()) {
        return false;
    }

    const TrieNode* current = root.get();

    for (unsigned char ch : prefix) {

        if (!std::isalpha(ch)) {
            return false;
        }

        char lowerChar =
            static_cast<char>(std::tolower(ch));

        int index =
            lowerChar - 'a';

        if (index < 0 || index >= 26) {
            return false;
        }

        if (!current->children[index]) {
            return false;
        }

        current =
            current->children[index].get();
    }

    return true;
}

//collectwords fun
void Trie::collectWords(
    const TrieNode* node,
    const std::string& currentWord,
    std::vector<std::string>& results
) const {

    if (node->isEndOfWord) {
        results.push_back(currentWord);
    }

    for (int i = 0; i < 26; ++i) {

        if (node->children[i]) {

            char nextChar =
                static_cast<char>('a' + i);

            collectWords(
                node->children[i].get(),
                currentWord + nextChar,
                results
            );
        }
    }
}

//Suggest fun
std::vector<std::string> Trie::suggest(
    const std::string& prefix
) const {

    std::vector<std::string> results;

    if (prefix.empty()) {
        return results;
    }

    const TrieNode* current = root.get();

    std::string normalizedPrefix;

    for (unsigned char ch : prefix) {

        if (!std::isalpha(ch)) {
            return {};
        }

        char lowerChar =
            static_cast<char>(std::tolower(ch));

        int index =
            lowerChar - 'a';

        if (index < 0 || index >= 26) {
            return {};
        }

        if (!current->children[index]) {
            return {};
        }

        current =
            current->children[index].get();

        normalizedPrefix += lowerChar;
    }

    collectWords(
        current,
        normalizedPrefix,
        results
    );

    return results;
}