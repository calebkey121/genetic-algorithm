#ifndef GENOME_H
#define GENOME_H

#include <vector>
#include <bitset>
#include <string>
#include <utility>

// (Move to constants.h later)
#define SOLUTION_SIZE 300
#define ADDRESS_SIZE 32
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
    std::bitset<SOLUTION_SIZE> Chromosome;
    std::vector<std::bitset<ADDRESS_SIZE>> addressVector;
    int Fitness;

    // Constructor
    Genome(std::bitset<SOLUTION_SIZE> chromosome, const std::vector<std::bitset<ADDRESS_SIZE>>& addrVec);

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