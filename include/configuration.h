#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>
#include <bitset>

// Forward declare for friend declaration
class ConfigurationBuilder;

/**
 * @class Configuration
 * @brief Centralized configuration for genetic algorithm parameters
 * 
 * This class replaces the global #define macros with configurable parameters
 * that can be adjusted at runtime. It follows an immutable design pattern with
 * a builder for construction.
 */
class Configuration {
private:
    // Algorithm parameters
    int populationSize;
    int elitismPercentage;
    float mutationChance;
    int maxGenerations;
    int solutionSize;
    int addressSize;
    int convergenceAllowance;
    int numExecutions;
    float segmentedCrossoverChance;
    std::string genePool;
    
    // Constructor is private - use ConfigurationBuilder instead
    Configuration(
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
    );
  
    // Friend declaration to allow builder access to private constructor
    friend class ConfigurationBuilder;

public:
    // Getters for configuration parameters
    int getPopulationSize() const { return populationSize; }
    int getElitismPercentage() const { return elitismPercentage; }
    float getMutationChance() const { return mutationChance; }
    int getMaxGenerations() const { return maxGenerations; }
    int getSolutionSize() const { return solutionSize; }
    int getAddressSize() const { return addressSize; }
    int getConvergenceAllowance() const { return convergenceAllowance; }
    int getNumExecutions() const { return numExecutions; }
    float getSegmentedCrossoverChance() const { return segmentedCrossoverChance; }
    const std::string& getGenePool() const { return genePool; }
    
    // Static method to get default configuration
    static Configuration getDefault();
};

/**
 * @class ConfigurationBuilder
 * @brief Builder pattern implementation for creating Configuration objects
 * 
 * This builder allows for fluent configuration of parameters with method chaining.
 * Each setter returns the builder itself to enable chaining.
 */
class ConfigurationBuilder {
private:
    int populationSize;
    int elitismPercentage;
    float mutationChance;
    int maxGenerations;
    int solutionSize;
    int addressSize;
    int convergenceAllowance;
    int numExecutions;
    float segmentedCrossoverChance;
    std::string genePool;

public:
    // Constructor with default values
    ConfigurationBuilder();
    
    // Setter methods with method chaining
    ConfigurationBuilder& withPopulationSize(int size);
    ConfigurationBuilder& withElitismPercentage(int percentage);
    ConfigurationBuilder& withMutationChance(float chance);
    ConfigurationBuilder& withMaxGenerations(int maxGen);
    ConfigurationBuilder& withSolutionSize(int size);
    ConfigurationBuilder& withAddressSize(int size);
    ConfigurationBuilder& withConvergenceAllowance(int allowance);
    ConfigurationBuilder& withNumExecutions(int executions);
    ConfigurationBuilder& withSegmentedCrossoverChance(float chance);
    ConfigurationBuilder& withGenePool(const std::string& genes);
    
    // Build method creates and returns the immutable Configuration
    Configuration build() const;
};

#endif // CONFIGURATION_H
