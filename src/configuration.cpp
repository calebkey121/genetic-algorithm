#include "Configuration.h"

// Private constructor implementation
Configuration::Configuration(
    int populationSize,
    int elitismPercentage,
    float mutationChance,
    int maxGenerations,
    int solutionSize,
    int addressSize,
    int convergenceAllowance,
    int numExecutions,
    float segmentedCrossoverChance,
    const std::string& genePool
) : 
    populationSize(populationSize),
    elitismPercentage(elitismPercentage),
    mutationChance(mutationChance),
    maxGenerations(maxGenerations),
    solutionSize(solutionSize),
    addressSize(addressSize),
    convergenceAllowance(convergenceAllowance),
    numExecutions(numExecutions),
    segmentedCrossoverChance(segmentedCrossoverChance),
    genePool(genePool)
{}

// Static method to get default configuration
Configuration Configuration::getDefault() {
    return ConfigurationBuilder().build();
}

// ConfigurationBuilder constructor with default values
ConfigurationBuilder::ConfigurationBuilder() :
    populationSize(100),              // POP_SIZE
    elitismPercentage(5),             // ELITISM_PERCENTAGE
    mutationChance(5.0f),             // MUTATION_CHANCE
    maxGenerations(400),              // MAX_GEN
    solutionSize(300),                // SOLUTION_SIZE
    addressSize(32),                  // ADDRESS_SIZE
    convergenceAllowance(25),         // CONVERGENCE_ALLOWENCE
    numExecutions(20),                // NUM_EXECUTIONS
    segmentedCrossoverChance(40.0f),  // SEG_X_CHANCE
    genePool("10")                    // genes
{}

// Builder setter implementations
ConfigurationBuilder& ConfigurationBuilder::withPopulationSize(int size) {
    populationSize = size;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::withElitismPercentage(int percentage) {
    elitismPercentage = percentage;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::withMutationChance(float chance) {
    mutationChance = chance;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::withMaxGenerations(int maxGen) {
    maxGenerations = maxGen;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::withSolutionSize(int size) {
    solutionSize = size;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::withAddressSize(int size) {
    addressSize = size;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::withConvergenceAllowance(int allowance) {
    convergenceAllowance = allowance;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::withNumExecutions(int executions) {
    numExecutions = executions;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::withSegmentedCrossoverChance(float chance) {
    segmentedCrossoverChance = chance;
    return *this;
}

ConfigurationBuilder& ConfigurationBuilder::withGenePool(const std::string& genes) {
    genePool = genes;
    return *this;
}

// Build method implementation
Configuration ConfigurationBuilder::build() const {
    return Configuration(
        populationSize,
        elitismPercentage,
        mutationChance,
        maxGenerations,
        solutionSize,
        addressSize,
        convergenceAllowance,
        numExecutions,
        segmentedCrossoverChance,
        genePool
    );
}