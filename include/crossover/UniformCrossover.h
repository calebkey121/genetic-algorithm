#ifndef UNIFORM_CROSSOVER_H
#define UNIFORM_CROSSOVER_H

#include "CrossoverStrategy.h"
#include "../utils/RandomUtils.h"

/**
 * @class UniformCrossover
 * @brief Implementation of uniform crossover strategy
 * 
 * In uniform crossover, each gene is randomly selected from either parent
 * with equal probability.
 */
class UniformCrossover : public CrossoverStrategy {
public:
    explicit UniformCrossover(const Configuration& config) : CrossoverStrategy(config) {}
    
    std::pair<Genome, Genome> crossover(
        const Genome& parent1, 
        const Genome& parent2
    ) const override;
    
    std::string getName() const override {
        return "UX";
    }
};

#endif // UNIFORM_CROSSOVER_H