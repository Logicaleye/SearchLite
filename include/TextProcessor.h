#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

class TextProcessor {
private:
    unordered_set<string> stopWords;

public:
    TextProcessor();

    string normalize(const string& text) const;

    vector<string> tokenize(const string& text) const;

    vector<string> process(const string& text) const;
};

#endif