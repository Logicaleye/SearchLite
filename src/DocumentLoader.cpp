#include "DocumentLoader.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

std::vector<Document> DocumentLoader::loadDocuments(
    const std::string& directoryPath
) const {
    std::vector<Document> documents;

    if (!fs::exists(directoryPath)) {
        throw std::runtime_error(
            "Directory does not exist: " + directoryPath
        );
    }

    int documentId = 1;

    for (const auto& entry : fs::directory_iterator(directoryPath)) {

        if (!entry.is_regular_file()) {
            continue;
        }

        if (entry.path().extension() != ".txt") {
            continue;
        }

        Document document;

        document.id = documentId++;
        document.path = entry.path().string();

        documents.push_back(document);
    }

    return documents;
}

std::string DocumentLoader::readDocument(
    const Document& document
) const {
    std::ifstream file(document.path);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Unable to open document: " + document.path
        );
    }

    std::string content;
    std::string line;

    while (std::getline(file, line)) {
        content += line;
        content += '\n';
    }

    return content;
}