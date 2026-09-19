#ifndef INDEX_PERSISTENCE_H
#define INDEX_PERSISTENCE_H

#include "InvertedIndex.h"

#include <string>

class IndexPersistence {
public:

    static bool save(
        const InvertedIndex& index,
        const std::string& filename
    );

    static bool load(
        InvertedIndex& index,
        const std::string& filename
    );
};

#endif