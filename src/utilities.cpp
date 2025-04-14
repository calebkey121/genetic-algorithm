// src/utilities.cpp
#include "utilities.h"
#include "constants.h"

#include <random>       // For modern C++ random numbers
#include <stdexcept>    // For exceptions
#include <algorithm>    // For std::shuffle
#include <chrono>       // For seeding random
#include <numeric>      // For std::accumulate (if needed elsewhere)
#include <filesystem>   // For directory creation
#include <iostream>     // For displayAddressVector, error messages
#include <iomanip>      // For std::setw in hex conversion

namespace Utils {

    // --- Random Number Generation Engine ---
    // Define a static generator instance within the cpp file
    static std::mt19937 rng_engine;
    static bool rng_seeded = false;

    void initialize_random_seed() {
        if (!rng_seeded) {
            // Seed with a combination of random_device and time for better randomness
            std::random_device rd;
            auto seed_val = static_cast<unsigned long>(rd()) ^
                            static_cast<unsigned long>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
            rng_engine.seed(seed_val);
            rng_seeded = true;
            std::cout << "INFO: Random number generator seeded." << std::endl;
        }
    }

    int randRange(int min, int max) {
        if (!rng_seeded) {
             // Or throw an exception, or seed here? Best practice is to seed explicitly once at program start.
             std::cerr << "WARNING: Random generator used before explicit seeding!\n";
             initialize_random_seed();
        }
        if (min > max) {
            throw std::invalid_argument("Min value cannot be greater than max value in randRange.");
        }
        // Use uniform_int_distribution for unbiased integer generation
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(rng_engine);
    }

    char randChar() {
         if (Constants::GENE_POOL.empty()) {
             throw std::runtime_error("Gene pool is empty.");
         }
         int index = randRange(0, static_cast<int>(Constants::GENE_POOL.length()) - 1);
         return Constants::GENE_POOL[index];
    }


    // --- Genome/Problem Specific Utilities ---

    std::string newGenome(std::size_t length) {
         std::string genome = "";
         genome.reserve(length);
         for (std::size_t i = 0; i < length; ++i) {
             genome += randChar();
         }
         return genome;
    }

    std::vector<std::bitset<Constants::ADDRESS_SIZE>> randAddressVector(std::size_t count, int locality_level) {
        // Simplified approach to simulate locality:
        // Generate a smaller pool of unique addresses, then sample from it with replacement,
        // occasionally repeating the last address used.

        if (count == 0) return {};

        std::vector<std::bitset<Constants::ADDRESS_SIZE>> addressVector;
        addressVector.reserve(count);

        // Use Bernoulli distribution for random bits
        std::bernoulli_distribution bit_dist(0.5); // 50% chance for 0 or 1

        // Create a smaller pool of base addresses
        std::size_t pool_size = std::max(static_cast<size_t>(1), count / std::max(1, locality_level));
        std::vector<std::bitset<Constants::ADDRESS_SIZE>> address_pool(pool_size);
        for(std::size_t i = 0; i < pool_size; ++i) {
             for (std::size_t n = 0; n < Constants::ADDRESS_SIZE; ++n) {
                 address_pool[i][n] = bit_dist(rng_engine);
             }
        }

        // Generate the final vector by sampling from the pool with repeats
        std::uniform_int_distribution<std::size_t> pool_sampler(0, pool_size - 1);
        std::bitset<Constants::ADDRESS_SIZE> last_addr; // Keep track of the last address

        for (std::size_t i = 0; i < count; ++i) {
             // Simple locality: Occasionally repeat the last address
             bool repeat_last = (i > 0) && (randRange(1, 10) <= locality_level); // Adjust probability as needed

             if (repeat_last) {
                  addressVector.push_back(last_addr);
             } else {
                  std::size_t pool_idx = pool_sampler(rng_engine);
                  last_addr = address_pool[pool_idx];
                  addressVector.push_back(last_addr);
             }
        }

         // Optionally shuffle the final vector slightly to mix repeats
         // std::shuffle(addressVector.begin(), addressVector.end(), rng_engine);

        return addressVector;
    }


    // --- Formatting and Display ---

    std::string addressToHex(std::bitset<Constants::ADDRESS_SIZE> address) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        // Process in chunks (e.g., 32 bits -> 8 hex chars)
        constexpr size_t bits_per_hex = 4;
        constexpr size_t hex_chars = (Constants::ADDRESS_SIZE + bits_per_hex - 1) / bits_per_hex; // Ceiling division

        for (int i = static_cast<int>(hex_chars) - 1; i >= 0; --i) {
            std::bitset<bits_per_hex> chunk;
            for (size_t j = 0; j < bits_per_hex; ++j) {
                size_t bit_index = i * bits_per_hex + j;
                if (bit_index < Constants::ADDRESS_SIZE) {
                    chunk[j] = address[bit_index];
                } else {
                     chunk[j] = 0; // Pad with 0 if ADDRESS_SIZE is not multiple of 4
                }
            }
            ss << chunk.to_ulong();
        }
        return ss.str();
    }

    void displayAddressVector(const std::vector<std::bitset<Constants::ADDRESS_SIZE>>& addressVector) {
         std::cout << "Address Vector (Size: " << addressVector.size() << "): <";
         bool first = true;
         for (const auto& addr : addressVector) {
             if (!first) {
                 std::cout << ", ";
             }
             std::cout << addressToHex(addr);
             first = false;
         }
         std::cout << ">" << std::endl;
    }

    // --- Filesystem Utilities ---
    bool createDirectory(const std::string& path_str) {
         std::filesystem::path dir_path(path_str);
         try {
             if (!std::filesystem::exists(dir_path)) {
                 if (std::filesystem::create_directories(dir_path)) {
                     std::cout << "INFO: Created directory: " << path_str << std::endl;
                     return true;
                 } else {
                     std::cerr << "ERROR: Failed to create directory: " << path_str << std::endl;
                     return false;
                 }
             } else if (!std::filesystem::is_directory(dir_path)) {
                  std::cerr << "ERROR: Path exists but is not a directory: " << path_str << std::endl;
                  return false;
             }
             return true; // Directory already exists
         } catch (const std::filesystem::filesystem_error& e) {
             std::cerr << "ERROR: Filesystem error creating directory " << path_str << ": " << e.what() << std::endl;
             return false;
         }
    }


}