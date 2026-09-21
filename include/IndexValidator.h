#ifndef INDEX_VALIDATOR_H
#define INDEX_VALIDATOR_H

#include "IndexMetadata.h"

#include <string>

class IndexValidator {

public:

    // Capture metadata for all .txt documents
    static IndexMetadata
    createMetadata(
        const std::string& directory
    );

    // Check whether current documents match
    // the metadata stored when the index was built
    static bool
    isValid(
        const std::string& directory,
        const IndexMetadata& metadata
    );
};

#endif