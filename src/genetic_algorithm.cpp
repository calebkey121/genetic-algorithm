// src/genetic_algorithm.cpp
#include "genetic_algorithm.h"
#include "genome.h"
#include "utilities.h" // For Utils::randRange, Utils::newGenome
#include "constants.h"

#include <algorithm> // For std::sort
#include <iostream>  // For std::cout (replace printf later)
#include <stdexcept>
#include <limits>

// C includes - Mark for removal later
#include <sys/stat.h> // For mkdir - replace with <filesystem>
#include <cstring>    // For strcpy - replace with std::string ops

// Constructor
GeneticAlgorithm::GeneticAlgorithm(std::string ga_name,
                                 const std::vector<std::bitset<ADDRESS_SIZE>>& addrVec,
                                 std::string crossover_type)
    : name(std::move(ga_name)), // Use std::move for strings
      selectedCrossoverType(std::move(crossover_type)),
      addressVector(addrVec), // Copy address vector
      currentGeneration(0),
      bestFitness(std::numeric_limits<int>::max()), // Initialize with worst possible fitness
      solutionFound(false),
      converged(false)
{
    // Validate crossover type
    if (selectedCrossoverType != "UX" && selectedCrossoverType != "2PX" &&
        selectedCrossoverType != "SX" && selectedCrossoverType != "BAX") {
        throw std::runtime_error("ERROR: Invalid crossover type selected: " + selectedCrossoverType);
    }
    initializePopulation();
}

// --- Initialization ---
void GeneticAlgorithm::initializePopulation() {
    population.reserve(POP_SIZE); // Reserve space for efficiency
    for (int i = 0; i < POP_SIZE; ++i) {
        // Create random bitset for chromosome
        std::string genomeString = Utils::newGenome(SOLUTION_SIZE); // Pass size
        std::bitset<SOLUTION_SIZE> genomeBits(genomeString);
        // Add new Genome to population (constructor calculates initial fitness)
        population.emplace_back(genomeBits, addressVector);
    }
    // Initial sort and fitness update
    sortPopulation();
    if (!population.empty()) {
         bestFitness = population.front().Fitness;
    }
}

// --- Core GA Loop ---
void GeneticAlgorithm::run() {
    int convergenceCounter = 0;
    int previousBestFitness = bestFitness;

    while (!solutionFound && !converged && currentGeneration < MAX_GEN) {

        sortPopulation();
        checkTerminationConditions(); // Check convergence and target fitness

        if (solutionFound || converged) {
            break; // Exit loop if termination conditions met
        }

        // --- Create Next Generation ---
        std::vector<Genome> newPopulation;
        newPopulation.reserve(POP_SIZE);

        performElitism(newPopulation);
        performSelectionAndCrossover(newPopulation);

        // Replace old population
        population = std::move(newPopulation); // Efficiently move data

         // --- Convergence Check ---
         if (bestFitness == previousBestFitness) {
             convergenceCounter++;
         } else {
             convergenceCounter = 0; // Reset if fitness improved
             previousBestFitness = bestFitness;
         }

         if (convergenceCounter >= CONVERGENCE_ALLOWENCE) {
             converged = true;
             std::cout << "INFO: GA converged after " << CONVERGENCE_ALLOWENCE
                       << " generations without fitness improvement." << std::endl;
         }

        // Advance Generation
        currentGeneration++;

    }

    // Final sort to ensure the best is at the front
    sortPopulation();
    if (!population.empty()) {
         bestFitness = population.front().Fitness; // Update final best fitness
    }

    std::cout << "INFO: GA run finished. Generation: " << currentGeneration
              << ", Best Fitness: " << bestFitness
              << ", Solution Found: " << (solutionFound ? "Yes" : "No")
              << ", Converged: " << (converged ? "Yes" : "No") << std::endl;
}

// --- Helper Methods ---
void GeneticAlgorithm::sortPopulation() {
    std::sort(population.begin(), population.end()); // Uses Genome::operator<
    if (!population.empty()) {
        bestFitness = population.front().Fitness; // Update best fitness after sort
    }
}

void GeneticAlgorithm::checkTerminationConditions() {
     if (population.empty()) {
         solutionFound = false; // Or handle as an error state
         converged = true; // Cannot proceed
         return;
     }
    // Check if target fitness (0) has been reached
    if (bestFitness == 0) {
        solutionFound = true;
        std::cout << "INFO: Target fitness (0) reached!" << std::endl;
    }
    // Max generation check is handled by the loop condition
}

void GeneticAlgorithm::performElitism(std::vector<Genome>& newPopulation) {
    int elitismCount = static_cast<int>((ELITISM_PERCENTAGE / 100.0) * POP_SIZE);
    elitismCount = std::min(elitismCount, static_cast<int>(population.size())); // Ensure not exceeding pop size

    for (int i = 0; i < elitismCount; ++i) {
        newPopulation.push_back(population[i]); // Copy elite individuals
    }
}

void GeneticAlgorithm::performSelectionAndCrossover(std::vector<Genome>& newPopulation) {
    int elitismCount = static_cast<int>((ELITISM_PERCENTAGE / 100.0) * POP_SIZE);
    int remainingPopulation = POP_SIZE - elitismCount;
    int parentPoolSize = POP_SIZE / 2; // Original logic selected from top 50%

    // Ensure parentPoolSize is valid
    parentPoolSize = std::max(1, std::min(parentPoolSize, static_cast<int>(population.size())));


    for (int i = 0; i < remainingPopulation; i += 2) { // Each iteration generates two offspring
        // Selection: Randomly choose two parents from the better half (top parentPoolSize)
        // Replace rand() later
        int parentA_idx = Utils::randRange(0, parentPoolSize - 1);
        int parentB_idx = Utils::randRange(0, parentPoolSize - 1);
        // Ensure parents are different
        while (parentA_idx == parentB_idx && parentPoolSize > 1) {
            parentB_idx = Utils::randRange(0, parentPoolSize - 1);
        }

        const Genome& parentA = population[parentA_idx];
        const Genome& parentB = population[parentB_idx];

        // Crossover
        std::pair<Genome, Genome> offspring;
        if (selectedCrossoverType == "UX") {
            offspring = parentA.UX(parentB);
        } else if (selectedCrossoverType == "2PX") {
            offspring = parentA.TWOPX(parentB);
        } else if (selectedCrossoverType == "SX") {
            offspring = parentA.SX(parentB);
        } else if (selectedCrossoverType == "BAX") {
            offspring = parentA.BAX(parentB);
        }
         // No need for else, constructor validated type

        // Add offspring to new population
        newPopulation.push_back(std::move(offspring.first)); // Move offspring
        if (newPopulation.size() < POP_SIZE) { // Add second offspring if space allows
            newPopulation.push_back(std::move(offspring.second));
        }
         // Ensure we don't exceed POP_SIZE, although the loop condition should prevent this if POP_SIZE - elitismCount is even.
         // If POP_SIZE - elitismCount is odd, the last iteration adds only one offspring.
         if (newPopulation.size() >= Constants::POP_SIZE) break;
    }
     // Handle potential size mismatch if POP_SIZE - elitismCount was odd
     while (newPopulation.size() < Constants::POP_SIZE && population.size() > 0) {
          // Fill remaining spots - e.g., repeat last crossover or select another parent
          int parentA_idx = Utils::randRange(0, parentPoolSize - 1);
          newPopulation.push_back(population[parentA_idx]); // Simple fill with a good parent
     }
}


// --- Getters ---
const Genome& GeneticAlgorithm::getBestGenome() const {
    if (population.empty()) {
        throw std::runtime_error("ERROR: Cannot get best genome from empty population.");
    }
    // Assumes population is sorted or was sorted finally in run()
    return population.front();
}

int GeneticAlgorithm::getCurrentGeneration() const {
    return currentGeneration;
}

 int GeneticAlgorithm::getBestFitness() const {
     if (population.empty()){
          return std::numeric_limits<int>::max(); // Or throw error
     }
     // Assumes population is sorted or bestFitness member is up-to-date
     return bestFitness; // Use member variable updated during run
 }

 bool GeneticAlgorithm::isSolutionFound() const {
     return solutionFound;
 }


// --- Display/Logging Methods (Implementation - Keep Simple or Move Out) ---

std::string GeneticAlgorithm::displayPopulationInfo() const {
    std::string info = "Generation: " + std::to_string(currentGeneration) + "\n";
    info += "Population Size: " + std::to_string(population.size()) + "\n";
    // Add more detailed population stats if needed (e.g., avg fitness)
    if (!population.empty()) {
         info += "Best Fitness in Pop: " + std::to_string(population.front().Fitness) + "\n";
         // info += "Worst Fitness in Pop: " + std::to_string(population.back().Fitness) + "\n";
         info += "Best Chromosome Preview: " + population.front().Chromosome.to_string().substr(0, 20) + "...\n";
    } else {
        info += "Population is empty.\n";
    }
    info += "**************************************************************\n";
    return info;
}

std::string GeneticAlgorithm::displaySolutionInfo() const {
     std::string info;
     if(!population.empty()){
          const Genome& best = getBestGenome(); // Get the best genome
          info += "Final Generation Reached: " + std::to_string(currentGeneration) + "\n";
          info += "Best Fitness Achieved: " + std::to_string(best.Fitness) + "\n";
          info += "Best Solution Chromosome: " + best.Chromosome.to_string() + "\n";
          info += "Converged: " + std::string(converged ? "Yes" : "No") + "\n";
          info += "Target Found: " + std::string(solutionFound ? "Yes" : "No") + "\n";
     } else {
          info += "No solution found - population is empty.\n";
     }
    return info;
}
