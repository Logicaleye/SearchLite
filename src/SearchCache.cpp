#include "SearchCache.h"

SearchCache::SearchCache(
    std::size_t capacity
)
    : capacity(capacity) {
}

bool SearchCache::contains(
    const std::string& query
) const {

    return cache.find(query)
        != cache.end();
}

bool SearchCache::get(
    const std::string& query,
    std::vector<int>& results
) {

    auto it = cache.find(query);

    if (it == cache.end()) {

    ++misses;

    return false;
}
++hits;

    results = it->second;

    // Move recently accessed query
    // to the front.
    order.remove(query);
    order.push_front(query);

    return true;
}

void SearchCache::put(
    const std::string& query,
    const std::vector<int>& results
) {

    // Cache disabled
    if (capacity == 0) {
        return;
    }

    auto it = cache.find(query);

    // Query already exists
    if (it != cache.end()) {

        it->second = results;

        order.remove(query);
        order.push_front(query);

        return;
    }

    // Evict least recently used query
    if (cache.size() >= capacity) {

        const std::string& oldest =
            order.back();

        cache.erase(oldest);
        order.pop_back();
    }

    cache[query] = results;

    order.push_front(query);
}

void SearchCache::clear() {

    cache.clear();
    order.clear();
}

std::size_t SearchCache::size() const {

    return cache.size();
}

std::size_t SearchCache::getHits() const {

    return hits;
}

std::size_t SearchCache::getMisses() const {

    return misses;
}

double SearchCache::getHitRate() const {

    const std::size_t total =
        hits + misses;

    if (total == 0) {
        return 0.0;
    }

    return static_cast<double>(hits) /
           static_cast<double>(total);
}