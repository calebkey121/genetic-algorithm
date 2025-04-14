#include "genome.h"
#include "lru_cache.h"
#include "utilities.h
#include <stdexcept>
#include <cstdlib>
#include <ctime>

// Constructor Implementation
Genome::Genome(std::bitset<Constants::SOLUTION_SIZE> chromosome, const std::vector<std::bitset<Constants::ADDRESS_SIZE>>& addrVec)
    : Chromosome(chromosome), addressVector(addrVec)
{
    Fitness = cal_fitness(); // Calculate fitness upon construction
}


// --- Crossover Implementations ---

std::pair<Genome, Genome> Genome::UX(Genome mate) {
    // Pseudo: Determine the reigons for inheritance
    //         Generate a random number to determine crossover
    //         Assign offspring gene based on probability - parent A, B, or mutate

    std::pair<std::bitset<Constants::SOLUTION_SIZE>, std::bitset<Constants::SOLUTION_SIZE>> offspringBitsets;

    // Reigons for inheritance
    // Use constexpr or const from constants.h later
    constexpr float mutationRate = static_cast<float>(Constants::MUTATION_RATE) / 100.0f;
    float parentRangeA = (1.0f - mutationRate) / 2.0f; // this parent (A)
    float parentRangeB = 1.0f - mutationRate;           // mate (B)

    for (int i = 0; i < Constants::SOLUTION_SIZE; i++) {
        // Replace with <random> later
        float p = static_cast<float>(Utils::randRange(0, 10000)) / 10000.0f; // Higher precision for probability

        if (p < parentRangeA) {
            offspringBitsets.first[i] = Chromosome[i];
            offspringBitsets.second[i] = mate.Chromosome[i];
        } else if (p < parentRangeB) {
            offspringBitsets.first[i] = mate.Chromosome[i];
            offspringBitsets.second[i] = Chromosome[i];
        } else { // Mutate
            // Use Utils::randChar() which should use <random> later
            offspringBitsets.first[i] = (Utils::randChar() == '1');
            offspringBitsets.second[i] = (Utils::randChar() == '1');
        }
    }
    // Pass addressVector by const reference
    return std::make_pair(Genome(offspringBitsets.first, addressVector), Genome(offspringBitsets.second, addressVector));
}

std::pair<Genome, Genome> Genome::TWOPX(Genome mate) {
    // https://stackoverflow.com/questions/7145583/two-point-crossover-operation
    std::pair<std::bitset<Constants::SOLUTION_SIZE>, std::bitset<Constants::SOLUTION_SIZE>> offspringBitsets;
    constexpr float mutationRate = static_cast<float>(Constants::MUTATION_RATE) / 100.0f;
    float mutationThreshold = 1.0f - mutationRate;

    // Generate two distinct random locations, making sure that B > A
    // Replace with <random> later
    int randLocationA = Utils::randRange(0, Constants::SOLUTION_SIZE - 2); // Ensure A can be < B
    int randLocationB = Utils::randRange(randLocationA + 1, Constants::SOLUTION_SIZE - 1); // Ensure B > A

    for (int i = 0; i < Constants::SOLUTION_SIZE; ++i) {
         // Replace with <random> later
        float p_mut = static_cast<float>(Utils::randRange(0, 10000)) / 10000.0f;

        if (p_mut >= mutationThreshold) { // Mutate
             offspringBitsets.first[i] = (Utils::randChar() == '1');
             offspringBitsets.second[i] = (Utils::randChar() == '1');
        } else { // Crossover
            if (i < randLocationA || i >= randLocationB) {
                // Outside crossover points: Offspring1 gets from Parent1 (this), Offspring2 from Parent2 (mate)
                offspringBitsets.first[i] = Chromosome[i];
                offspringBitsets.second[i] = mate.Chromosome[i];
            } else {
                // Inside crossover points: Offspring1 gets from Parent2 (mate), Offspring2 from Parent1 (this)
                offspringBitsets.first[i] = mate.Chromosome[i];
                offspringBitsets.second[i] = Chromosome[i];
            }
        }
    }

    return std::make_pair(Genome(offspringBitsets.first, addressVector), Genome(offspringBitsets.second, addressVector));
}

std::pair<Genome, Genome> Genome::SX(Genome mate) {
     std::pair<std::bitset<Constants::SOLUTION_SIZE>, std::bitset<Constants::SOLUTION_SIZE>> offspringBitsets;

    constexpr float mutationRate = static_cast<float>(Constants::MUTATION_RATE) / 100.0f;
    constexpr float segRate = static_cast<float>(Constants::SEG_X_SWITCH_RATE) / 100.0f;

    float mutationThreshold = 1.0f - mutationRate;
    float segmentationThreshold = segRate; // Chance to switch parent dominance

    bool parent1Dominant = true; // Start with parent1 (this) dominating offspring1

    for (int i = 0; i < Constants::SOLUTION_SIZE; i++) {
        float p_mut = static_cast<float>(Utils::randRange(0, 10000)) / 10000.0f;
        float p_seg = static_cast<float>(Utils::randRange(0, 10000)) / 10000.0f;

        if (p_mut >= mutationThreshold) { // Mutate
            offspringBitsets.first[i] = (Utils::randChar() == '1');
            offspringBitsets.second[i] = (Utils::randChar() == '1');
        } else {
            // Check for segment switch *before* assigning based on dominance
             if (p_seg < segmentationThreshold) {
                parent1Dominant = !parent1Dominant; // Switch dominance
             }

            // Assign genes based on current dominance
            if (parent1Dominant) {
                offspringBitsets.first[i] = Chromosome[i];        // Offspring1 gets from Parent1 (this)
                offspringBitsets.second[i] = mate.Chromosome[i]; // Offspring2 gets from Parent2 (mate)
            } else {
                offspringBitsets.first[i] = mate.Chromosome[i]; // Offspring1 gets from Parent2 (mate)
                offspringBitsets.second[i] = Chromosome[i];        // Offspring2 gets from Parent1 (this)
            }
        }
    }

    return std::make_pair(Genome(offspringBitsets.first, addressVector), Genome(offspringBitsets.second, addressVector));
}


std::pair<Genome, Genome> Genome::BAX(Genome mate) {
    // Simplified BAX: Chooses between UX and TWOPX based on some criteria
    // Example criteria: difference in fitness, or a random choice.

    // Replace with <random> later
    float p = static_cast<float>(Utils::randRange(0, 100)) / 100.0f;
    if (p < 0.5f) {
        return this->UX(mate);
    } else {
        return this->TWOPX(mate);
    }
}

// Fitness Calculation
int Genome::cal_fitness() {
    // Fitness score = the number of cache evictions. Lower is better.
    LRUCache cache; // Create a cache instance for fitness evaluation
    int fitness = 0; // Lower is better (fewer evictions)

    // Check if addressVector is populated
    if (addressVector.empty() || addressVector.size() != 100) {
         // Handle error: address vector not properly initialized or wrong size
         // Return a very high fitness score to penalize this genome
         // Or throw an exception, depending on desired behavior
         return 1000000; // Penalize heavily
    }

    // Ensure chromosome size matches expected indexing logic (100 addresses * 3 bits/index = 300 bits)
    if (Constants::SOLUTION_SIZE != 300) {
         // Handle error: SOLUTION_SIZE incompatible with fitness function
         return 1000001; // Penalize heavily
    }


    std::bitset indexBits;
    for (int i = 0; i < 100; ++i) {
        // Check array bounds carefully
        int base_idx = i * 3;
        if (base_idx + 2 >= Constants::SOLUTION_SIZE) {
             // Handle error: Chromosome too short for this loop iteration
             return 1000002; // Penalize
        }

        // Extract 3 bits for the index
        indexBits[0] = Chromosome[base_idx];
        indexBits[1] = Chromosome[base_idx + 1];
        indexBits[2] = Chromosome[base_idx + 2];

        // Convert bits to an integer index (0-7)
        int setIndex = static_cast<int>(indexBits.to_ulong());

        // Access the cache using the calculated index and the address
        // The index method in LRUCache should return true if an eviction occurred
        cache.index(setIndex, addressVector.at(i)); // Use .at() for bounds checking
    }

    fitness = cache.getEvictionCount();
    return fitness;
}

// Comparison Operator
bool Genome::operator<(const Genome& other) const {
    // Lower fitness is better
    return Fitness < other.Fitness;
}