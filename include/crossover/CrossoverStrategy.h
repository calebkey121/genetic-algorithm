// CrossoverStrategy.h
#ifndef CROSSOVER_STRATEGY_H
#define CROSSOVER_STRATEGY_H

#include <bitset>
#include <utility>
#include <string>
#include "../Configuration.h"
#include "../Genome.h"

// Forward declaration to resolve circular dependency
class Genome;

/**
 * @class CrossoverStrategy
 * @brief Abstract base class for crossover operations
 * 
 * Defines the interface for all crossover strategies. Each strategy
 * implements a different method of genetic crossover between two parent
 * genomes to produce offspring.
 */
class CrossoverStrategy {
protected:
    const Configuration& config;
    
public:
    /**
     * Constructor
     * @param config Reference to the algorithm configuration
     */
    explicit CrossoverStrategy(const Configuration& config) : config(config) {}
    
    /**
     * Virtual destructor for proper inheritance
     */
    virtual ~CrossoverStrategy() = default;
    
    /**
     * Performs crossover between two parent genomes
     * @param parent1 First parent genome
     * @param parent2 Second parent genome
     * @return A pair of offspring genomes
     */
    virtual std::pair<Genome, Genome> crossover(
        const Genome& parent1, 
        const Genome& parent2
    ) const = 0;
    
    /**
     * Gets the name of this crossover strategy
     * @return String identifier for the strategy
     */
    virtual std::string getName() const = 0;
};

#endif // CROSSOVER_STRATEGY_H