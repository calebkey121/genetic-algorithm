// src/lru_cache.cpp
#include "lru_cache.h"
#include "utilities.h"
#include "constants.h"

#include <stdexcept> // For bounds checking errors
#include <iomanip>   // For formatting output if needed
#include <filesystem> // Include later for directory creation
#include <iostream>   // for cerr


// --- LRUSet Method Implementations ---
LRUSet::LRUSet(size_t ways) : capacity(ways) {
    if (capacity == 0) {
        throw std::invalid_argument("Cache set capacity (ways) must be greater than 0.");
    }
}

bool LRUSet::refer(const Address& addr) {
    bool eviction = false;

    // Check if address is already in the cache (cache hit)
    auto it = cacheMap.find(addr);
    if (it != cacheMap.end()) {
        // Hit: Move the accessed element to the front (MRU)
        cacheList.splice(cacheList.begin(), cacheList, it->second);
    } else {
        // Miss: Check if the cache set is full
        if (cacheList.size() >= capacity) {
            // Eviction needed: Remove the least recently used element (back of the list)
            Address lru_addr = cacheList.back();
            cacheMap.erase(lru_addr); // Remove from map
            cacheList.pop_back();    // Remove from list
            eviction = true;
        }
        // Add the new element to the front (MRU)
        cacheList.push_front(addr);
        cacheMap[addr] = cacheList.begin(); // Store iterator in map
    }
    return eviction;
}

std::string LRUSet::display() const {
    std::stringstream ss;
    ss << "|";
    size_t count = 0;
    for (const auto& addr : cacheList) {
        ss << " " << std::setw(8) << std::left << Utils::addressToHex(addr) << " |"; // Use utility
        count++;
    }
    // Fill remaining slots if set is not full for consistent width
    for (size_t i = count; i < capacity; ++i) {
         ss << " " << std::setw(8) << std::left << "--------" << " |";
    }
    // ss << " (Size: " << cacheList.size() << ")"; // Optional: Show current occupancy
    return ss.str();
}

 void LRUSet::clear() {
     cacheList.clear();
     cacheMap.clear();
 }

 size_t LRUSet::getSize() const {
     return cacheList.size();
 }


// --- LRUCache Method Implementations ---

LRUCache::LRUCache(size_t num_sets, size_t ways_per_set)
    : numSets(num_sets), ways(ways_per_set), evictionCount(0) {
    if (numSets == 0) {
        throw std::invalid_argument("Number of cache sets must be greater than 0.");
    }
    // Initialize the vector of sets
    sets.assign(numSets, LRUSet(ways)); // Create numSets copies of LRUSet with 'ways' capacity
}

void LRUCache::index(int set_index, const std::bitset<Constants::ADDRESS_SIZE>& value) {
    // Bounds check for the set index
    if (set_index < 0 || static_cast<size_t>(set_index) >= numSets) {
        throw std::out_of_range("Cache set index out of range. Index: " + std::to_string(set_index) + ", Num Sets: " + std::to_string(numSets));
    }

    // Call refer on the appropriate set and update eviction count if necessary
    if (sets[set_index].refer(value)) {
        evictionCount++;
    }
}

int LRUCache::getEvictionCount() const {
    return evictionCount;
}

void LRUCache::reset() {
    evictionCount = 0;
    for (LRUSet& set : sets) {
        set.clear();
    }
}

std::string LRUCache::display() const {
    std::stringstream ss;
    ss << "--- Cache State (Evictions: " << evictionCount << ") ---\n";
    for (size_t i = 0; i < numSets; ++i) {
        ss << "Set " << std::setw(2) << i << ": " << sets[i].display() << "\n";
    }
    ss << "---------------------------------------\n";
    return ss.str();
}

// Static simulation method implementation
void LRUCache::simulateCache(const std::string& name,
                             const std::string& crossoverType,
                             const std::bitset<Constants::SOLUTION_SIZE>& chromosome,
                             const std::vector<std::bitset<Constants::ADDRESS_SIZE>>& addressVector)
{
    // --- File Output Setup ---
    // Replace with <filesystem> path joining later for robustness
    std::string simulationDir = crossoverType + "_Simulation";
    std::string filePath = simulationDir + "/Simulation - " + name + ".txt";

    // Create directory - Replace with <filesystem> later
    // #include <sys/stat.h> // Needed for mkdir - temporary
    // mkdir(simulationDir.c_str(), 0777); // Basic directory creation

    std::ofstream simulation_out(filePath);
    if (!simulation_out) {
         std::cerr << "ERROR: Could not open simulation output file: " << filePath << std::endl;
         return; // Or throw exception
    }

    simulation_out << "******* Cache Simulation - GA: " << name << " (" << crossoverType << ") *******\n";
    simulation_out << "Chromosome: " << chromosome.to_string() << "\n";
    simulation_out << "Address Trace Size: " << addressVector.size() << "\n";
    simulation_out << "**********************************************************\n\n";

    // --- Simulation ---
    LRUCache cache(Constants::DEFAULT_CACHE_SETS, Constants::DEFAULT_CACHE_WAYS); // Use default geometry for simulation

    if (addressVector.size() != 100 || Constants::SOLUTION_SIZE != 300) {
         simulation_out << "ERROR: Incompatible address vector size (" << addressVector.size()
                        << ") or chromosome size (" << Constants::SOLUTION_SIZE
                        << ") for standard simulation (expects 100 addresses, 300 bits).\n";
         return;
    }

    std::bitset<3> indexBits; // Assuming 3 bits = log2(DEFAULT_CACHE_SETS) = log2(8)
    for (size_t i = 0; i < addressVector.size(); ++i) {
        // Extract index bits from chromosome
        int base_idx = static_cast<int>(i * 3);
         if (base_idx + 2 >= Constants::SOLUTION_SIZE) {
             simulation_out << "ERROR: Chromosome index out of bounds at address " << i << "\n";
             break; // Stop simulation on error
         }
        indexBits[0] = chromosome[base_idx];
        indexBits[1] = chromosome[base_idx + 1];
        indexBits[2] = chromosome[base_idx + 2];
        int setIndex = static_cast<int>(indexBits.to_ulong());

         simulation_out << "Step " << std::setw(3) << (i + 1) << ": Address=" << Utils::addressToHex(addressVector.at(i))
                        << ", RawIndexBits=" << indexBits.to_string() << ", SetIndex=" << setIndex << "\n";

        // Access the cache
        cache.index(setIndex, addressVector.at(i));

        // Log cache state
        simulation_out << cache.display(); // Display state after access
        simulation_out << "---------------------------------------\n";
    }

    simulation_out << "\n******* Simulation Complete *******\n";
    simulation_out << "Final Eviction Count: " << cache.getEvictionCount() << "\n";
    simulation_out << "***********************************\n";

    simulation_out.close(); // Good practice to close the file stream
}
