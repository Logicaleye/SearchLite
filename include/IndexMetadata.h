#ifndef INDEX_METADATA_H
#define INDEX_METADATA_H

#include <string>
#include <unordered_map>

class IndexMetadata {

private:

    // Document path -> last modification timestamp
    std::unordered_map<std::string, long long> documentTimestamps;

public:

    // Record the modification time of a document
    void addDocument(
        const std::string& path,
        long long timestamp
    );

    // Check whether a document exists in the metadata
    bool contains(
        const std::string& path
    ) const;

    // Get stored timestamp
    long long getTimestamp(
        const std::string& path
    ) const;

    // Get all stored document metadata
    const std::unordered_map<std::string, long long>&
    getDocuments() const;

    // Number of documents stored
    std::size_t size() const;
};

#endif