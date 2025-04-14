#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <string>
#include <bitset>
#include <sstream>
#include "constants.h"

namespace Utils {

    // --- Random Number Generation ---
    // Initialize the random number generator (declaration)
    void initialize_random_seed();
    // Generate a random integer in a range [min, max] (inclusive)
    int randRange(int min, int max);
    // Generate a random character from the defined gene pool
    char randChar();

    // --- Genome/Problem Specific Utilities ---
    // Create a random genome string of specified length
    std::string newGenome(std::size_t length = Constants::SOLUTION_SIZE);
    // Generate a vector of random addresses (simulating locality)
    std::vector<std::bitset<Constants::ADDRESS_SIZE>> randAddressVector(std::size_t count = 100, int locality_level = 4);

    // --- Formatting and Display ---
    // Convert an address bitset to a hex string
    std::string addressToHex(std::bitset<Constants::ADDRESS_SIZE> address);
    // Display the address vector (for debugging)
    void displayAddressVector(const std::vector<std::bitset<Constants::ADDRESS_SIZE>>& addressVector);

    // --- Filesystem Utilities ---
    // Creates a directory if it doesn't exist. Returns true on success.
    bool createDirectory(const std::string& path);


}

#endif // UTILITIES_H