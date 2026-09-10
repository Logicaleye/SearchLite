#ifndef TRIE_H
#define TRIE_H

#include <array>
#include <memory>
#include <string>
#include <vector>

struct TrieNode {
    std::array<std::unique_ptr<TrieNode>, 26> children;
    bool isEndOfWord;

    TrieNode();
};

class Trie {
private:
    std::unique_ptr<TrieNode> root;

    void collectWords(
        const TrieNode* node,
        const std::string& currentWord,
        std::vector<std::string>& results
    ) const;

public:
    Trie();

    void insert(const std::string& word);

    bool search(const std::string& word) const;

    bool startsWith(const std::string& prefix) const;

    std::vector<std::string> suggest(
        const std::string& prefix
    ) const;
};

#endif