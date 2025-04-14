#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include <list>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <string>
#include <sstream>
#include <fstream>
#include "constants.h"

// Forward declare utility function if needed directly
namespace Utils {
    std::string addressToHex(std::bitset<Constants::ADDRESS_SIZE> address);
}

// --- LRUSet Class (Represents one set in the cache) ---
class LRUSet {
private:
    using Address = std::bitset<Constants::ADDRESS_SIZE>;
    using ListIterator = std::list<Address>::iterator;

    std::list<Address> cacheList; // Stores addresses in LRU order (front = MRU, back = LRU)
    std::unordered_map<Address, ListIterator> cacheMap; // Maps address to its position in the list
    size_t capacity; // Number of ways (blocks) in this set

public:
    explicit LRUSet(size_t ways); // Constructor takes capacity (ways)
    LRUSet() : LRUSet(Constants::DEFAULT_CACHE_WAYS) {} // Default constructor if needed

    bool refer(const Address& addr); // Returns true if eviction occurred
    std::string display() const;     // Returns string representation of the set
    void clear();                    // Clears the set
    size_t getSize() const;          // Returns current number of items in set
};

// --- LRUCache Class (Represents the entire cache) ---
class LRUCache {
private:
    std::vector<LRUSet> sets; // Array of cache sets
    size_t numSets;
    size_t ways;
    int evictionCount;

public:
    // Constructor takes number of sets and ways per set
    LRUCache(size_t num_sets = Constants::DEFAULT_CACHE_SETS, size_t ways_per_set = Constants::DEFAULT_CACHE_WAYS);

    void index(int set_index, const std::bitset<Constants::ADDRESS_SIZE>& value); // Access cache
    int getEvictionCount() const;
    void reset();                 // Reset cache state
    std::string display() const; // Display entire cache state

    // Static simulation method (consider moving this outside later)
    static void simulateCache(const std::string& name,
                              const std::string& crossoverType,
                              const std::bitset<Constants::SOLUTION_SIZE>& chromosome,
                              const std::vector<std::bitset<Constants::ADDRESS_SIZE>>& addressVector);
};

#endif // LRU_CACHE_H