#ifndef DOCUMENT_LOADER_H
#define DOCUMENT_LOADER_H
#include "Document.h"
#include <string>
#include <vector>

class DocumentLoader {
public:
    std::vector<Document> loadDocuments(
        const std::string& directoryPath
    ) const;

    std::string readDocument(
        const Document& document
    ) const;
};

#endif