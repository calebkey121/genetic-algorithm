#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include <bitset>
#include <string>
#include <utility>
#include "constants.h"

#define MUTATION_CHANCE 5
#define SEG_X_CHANCE 40
const std::string genes = "10";

// Forward declaration to avoid circular include if LRUCache needs Genome
class LRUCache;
namespace Utils {
    char randChar();
}

class Genome {
public:
    std::bitset<Constants::SOLUTION_SIZE> Chromosome;
    std::vector<std::bitset<Constants::ADDRESS_SIZE>> addressVector;
    int Fitness;

    // Constructor
    Genome(std::bitset<Constants::SOLUTION_SIZE> chromosome, const std::vector<std::bitset<Constants::ADDRESS_SIZE>>& addrVec);

    // Fitness calculation
    int cal_fitness();

    // Crossover operators
    std::pair<Genome, Genome> UX(Genome mate);
    std::pair<Genome, Genome> TWOPX(Genome mate);
    std::pair<Genome, Genome> SX(Genome mate);
    std::pair<Genome, Genome> BAX(Genome mate);

    // Comparison operator for sorting
    bool operator<(const Genome& other) const; // Mark as const
};

#endif // GENOME_H