#include "IndexMetadata.h"

void IndexMetadata::addDocument(
    const std::string& path,
    long long timestamp
) {
    documentTimestamps[path] = timestamp;
}

bool IndexMetadata::contains(
    const std::string& path
) const {
    return documentTimestamps.find(path)
        != documentTimestamps.end();
}

long long IndexMetadata::getTimestamp(
    const std::string& path
) const {

    auto it =
        documentTimestamps.find(path);

    if (it == documentTimestamps.end()) {
        return -1;
    }

    return it->second;
}

const std::unordered_map<std::string, long long>&
IndexMetadata::getDocuments() const {
    return documentTimestamps;
}

std::size_t IndexMetadata::size() const {
    return documentTimestamps.size();
}