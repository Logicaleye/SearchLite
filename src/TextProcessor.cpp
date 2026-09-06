#include "TextProcessor.h"

#include <algorithm>
#include <cctype>
#include <sstream>

using namespace std;

TextProcessor::TextProcessor() {
    stopWords = {
        "a", "an", "the",
        "is", "are", "was", "were",
        "of", "to", "in", "on",
        "for", "and", "or"
    };
}

string TextProcessor::normalize(const string& text) const {
    string result;

    for (unsigned char ch : text) {
        if (isalnum(ch) || isspace(ch)) {
            result += static_cast<char>(tolower(ch));
        } else {
            result += ' ';
        }
    }

    return result;
}

vector<string> TextProcessor::tokenize(
    const string& text
) const {
    vector<string> tokens;
    stringstream stream(text);

    string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

vector<string> TextProcessor::process(
    const string& text
) const {
    string normalized = normalize(text);

    vector<string> tokens = tokenize(normalized);

    vector<string> result;

    for (const string& token : tokens) {
        if (stopWords.find(token) == stopWords.end()) {
            result.push_back(token);
        }
    }

    return result;
}