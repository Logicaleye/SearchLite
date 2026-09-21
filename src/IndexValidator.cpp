#include "IndexValidator.h"

#include <filesystem>

namespace fs = std::filesystem;

IndexMetadata
IndexValidator::createMetadata(
    const std::string& directory
) {
    IndexMetadata metadata;

    if (!fs::exists(directory)) {
        return metadata;
    }

    for (
        const auto& entry :
        fs::directory_iterator(directory)
    ) {

        if (!entry.is_regular_file()) {
            continue;
        }

        if (entry.path().extension() != ".txt") {
            continue;
        }

        auto fileTime =
            fs::last_write_time(entry.path());

        auto timestamp =
            fileTime.time_since_epoch().count();

        metadata.addDocument(
            entry.path().string(),
            static_cast<long long>(timestamp)
        );
    }

    return metadata;
}

bool
IndexValidator::isValid(
    const std::string& directory,
    const IndexMetadata& metadata
) {
    IndexMetadata current =
        createMetadata(directory);

    // Number of documents changed
    if (current.size() != metadata.size()) {
        return false;
    }

    // Compare every current document
    for (
        const auto& [path, timestamp] :
        current.getDocuments()
    ) {

        if (!metadata.contains(path)) {
            return false;
        }

        if (
            metadata.getTimestamp(path)
            != timestamp
        ) {
            return false;
        }
    }

    return true;
}