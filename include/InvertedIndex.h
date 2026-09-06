#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
class InvertedIndex{
    private:
        unordered_map<string, vector<int>> index;

    public:
        void add(const string& term, int documentId);

        vector<int> search(const string& term) const;

        bool contains(const string& term) const;

        size_t size() const;
};
#endif