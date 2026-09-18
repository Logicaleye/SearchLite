#ifndef SEARCH_CACHE_H
#define SEARCH_CACHE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <list>

class SearchCache {
std::size_t hits = 0;
std::size_t misses = 0;
private:

    // Query → search results
    std::unordered_map<
        std::string,
        std::vector<int>
    > cache;

    // Maintains query usage order
    std::list<std::string> order;

    // Maximum number of cached queries
    std::size_t capacity;

public:

    explicit SearchCache(
        std::size_t capacity = 100
    );

    bool contains(
        const std::string& query
    ) const;

    bool get(
        const std::string& query,
        std::vector<int>& results
    );

    void put(
        const std::string& query,
        const std::vector<int>& results
    );

    void clear();

    std::size_t size() const;

    std::size_t getHits() const;

    std::size_t getMisses() const;

    double getHitRate() const;
};

#endif