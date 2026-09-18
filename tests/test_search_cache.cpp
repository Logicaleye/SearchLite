#include "SearchCache.h"

#include <cassert>
#include <iostream>
#include <vector>

int main() {

    // Test 1: Empty cache
    {
        SearchCache cache(3);

        assert(cache.size() == 0);
        assert(!cache.contains("machine"));
    }

    // Test 2: Insert and retrieve
    {
        SearchCache cache(3);

        cache.put(
            "machine",
            {1, 2, 3}
        );

        std::vector<int> results;

        bool found =
            cache.get(
                "machine",
                results
            );

        assert(found);
        assert(
            (results == std::vector<int>{1, 2, 3})
        );
    }

    // Test 3: Cache miss
    {
        SearchCache cache(3);

        std::vector<int> results;

        bool found =
            cache.get(
                "unknown",
                results
            );

        assert(!found);
    }

    // Test 4: Update existing query
    {
        SearchCache cache(3);

        cache.put(
            "machine",
            {1, 2}
        );

        cache.put(
            "machine",
            {3}
        );

        std::vector<int> results;

        assert(
            cache.get(
                "machine",
                results
            )
        );

        assert(
            (results == std::vector<int>{3})
        );

        assert(cache.size() == 1);
    }

    // Test 5: LRU eviction
    {
        SearchCache cache(2);

        cache.put("machine", {1});
        cache.put("learning", {2});

        // machine becomes recently used
        std::vector<int> results;

        assert(
            cache.get(
                "machine",
                results
            )
        );

        // Insert third query.
        cache.put(
            "computer",
            {3}
        );

        // learning should be evicted.
        assert(
            cache.contains("machine")
        );

        assert(
            cache.contains("computer")
        );

        assert(
            !cache.contains("learning")
        );
    }

    // Test 6: Clear
    {
        SearchCache cache(3);

        cache.put("machine", {1});
        cache.put("learning", {2});

        cache.clear();

        assert(cache.size() == 0);
        assert(!cache.contains("machine"));
        assert(!cache.contains("learning"));
    }

    // Test 7: Zero capacity
    {
        SearchCache cache(0);

        cache.put(
            "machine",
            {1}
        );

        assert(cache.size() == 0);
        assert(!cache.contains("machine"));
    }

    // Test 8: Cache statistics
{
    SearchCache cache(3);

    std::vector<int> results;

    // Miss
    assert(
        !cache.get(
            "machine",
            results
        )
    );

    // Insert
    cache.put(
        "machine",
        {1, 2}
    );

    // Hit
    assert(
        cache.get(
            "machine",
            results
        )
    );

    // Another hit
    assert(
        cache.get(
            "machine",
            results
        )
    );

    assert(cache.getHits() == 2);
    assert(cache.getMisses() == 1);

    assert(
        cache.getHitRate() >
        0.66
    );
}

    std::cout
        << "All SearchCache tests passed!"
        << std::endl;

    return 0;
}