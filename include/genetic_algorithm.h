#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

#include <vector>
#include <string>
#include <fstream>
#include <bitset>
#include <memory>
#include "constants.h"

// Forward declare Genome to avoid including its header here
// if only pointers or references are needed. Full include needed for vector<Genome>.
#include "genome.h" // Including because std::vector<Genome> population; requires the full type

class GeneticAlgorithm {
public:
    // Constructor: Takes necessary parameters
    GeneticAlgorithm(std::string ga_name,
                     const std::vector<std::bitset<Constants::ADDRESS_SIZE>>& addrVec,
                     std::string crossover_type);

    // --- Core GA Methods ---
    void run(); // Runs the genetic algorithm loop
    // void initializePopulation();
    // void evaluatePopulation();
    // std::vector<Genome> selectParents();
    // void createNextGeneration();

    // --- Getters for Results ---
    const Genome& getBestGenome() const; // Return const reference to best genome
    int getCurrentGeneration() const;
    int getBestFitness() const;
    bool isSolutionFound() const;

    // --- Optional: Display/Logging Methods (Consider moving logic outside) ---
    std::string displayPopulationInfo() const; // Renamed from displayPop
    std::string displaySolutionInfo() const; // Renamed from displaySolution

private:
    // --- Member Variables ---
    std::string name;
    std::string selectedCrossoverType;
    std::vector<std::bitset<Constants::ADDRESS_SIZE>> addressVector; // Keep a const ref? Depends if GA modifies it (it shouldn't)
    std::vector<Genome> population;
    int currentGeneration;
    int bestFitness;
    bool solutionFound;
    bool converged;

    // --- Helper Methods (Private) ---
    void initializePopulation();
    void sortPopulation();
    void performElitism(std::vector<Genome>& newPopulation);
    void performSelectionAndCrossover(std::vector<Genome>& newPopulation);
    void checkTerminationConditions();
};

#endif // GENETIC_ALGORITHM_H