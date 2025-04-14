#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <cstddef>

namespace Constants {

    // === Genetic Algorithm Parameters ===

    // Population settings
    constexpr std::size_t POP_SIZE = 100;           // Number of individuals in the population
    constexpr double ELITISM_RATE = 0.05;           // Top 5% of population carried over (0.0 to 1.0)

    // Mutation and Crossover settings
    constexpr double MUTATION_RATE = 0.05;          // Chance (0.0 to 1.0) for a gene to mutate during crossover/mutation step
    constexpr double SEG_X_SWITCH_RATE = 0.40;      // Chance (0.0 to 1.0) to switch parent dominance in Segmented Crossover (SX)

    // Termination conditions
    constexpr int MAX_GEN = 400;                    // Maximum number of generations to run
    constexpr int CONVERGENCE_ALLOWENCE = 25;       // # of generations with no fitness improvement to declare convergence

    // Genome/Problem definition
    constexpr std::size_t SOLUTION_SIZE = 300;      // Chromosome length in bits (e.g., 100 addresses * 3 bits/index)
    constexpr std::size_t ADDRESS_SIZE = 32;        // Address length in bits
    const std::string GENE_POOL = "01";             // Allowed characters/bits for chromosome generation


    // === LRU Cache Parameters ===

    constexpr std::size_t DEFAULT_CACHE_SETS = 8;   // Default number of sets in the simulated cache (log2(8)=3 index bits)
    constexpr std::size_t DEFAULT_CACHE_WAYS = 2;   // Default number of ways (associativity) per set


    // === Experiment Parameters ===

    constexpr int NUM_EXECUTIONS = 20;              // Default number of times to run the GA experiment for averaging results


    // === Other Configuration ===

    // Selection Pressure (used in original random selection from top half)
    // We can make this explicit if we refactor selection later
    constexpr double SELECTION_POOL_RATIO = 0.50;   // Select parents from the top 50% of the sorted population

}

#endif // CONSTANTS_H