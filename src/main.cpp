// src/main.cpp
#include <iostream>
#include <vector>
#include <string>
#include <chrono>    // For timing
#include <numeric>   // For std::accumulate
#include <algorithm> // For std::minmax_element
#include <fstream>   // For file output
#include <iomanip>   // For formatting output
#include <filesystem> // For path handling

// Include project headers
#include "constants.h"
#include "utilities.h"
#include "genome.h"
#include "genetic_algorithm.h"
#include "lru_cache.h"

// Function to run the experiment for a given crossover type
void run_experiment(const std::string& crossover_type,
                   const std::vector<std::bitset<Constants::ADDRESS_SIZE>>& address_vector)
{
    std::cout << "\n===== Running Experiment for Crossover Type: " << crossover_type << " =====\n";

    // --- Setup Output ---
    std::string results_dir = "Results";
    std::string ga_solution_dir = crossover_type + "_GA_Solution";
    std::string simulation_dir = crossover_type + "_Simulation";

    if (!Utils::createDirectory(results_dir) ||
        !Utils::createDirectory(ga_solution_dir) ||
        !Utils::createDirectory(simulation_dir))
    {
        std::cerr << "ERROR: Failed to create necessary output directories. Aborting experiment for " << crossover_type << "." << std::endl;
        return;
    }

    std::string results_file_path = results_dir + "/" + crossover_type + "_Results.txt";
    std::ofstream results_out(results_file_path);
    if (!results_out) {
        std::cerr << "ERROR: Cannot open results file: " << results_file_path << std::endl;
        return;
    }

    auto now_time = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now_time);

    results_out << "********** Experiment Results for Crossover: " << crossover_type << " **********\n";
    results_out << "Timestamp: " << std::ctime(&now_c);
    results_out << "Number of Executions: " << Constants::NUM_EXECUTIONS << "\n";
    results_out << "Population Size: " << Constants::POP_SIZE << "\n";
    results_out << "Max Generations: " << Constants::MAX_GEN << "\n";
    results_out << "Mutation Rate: " << Constants::MUTATION_RATE << "\n";
    results_out << "Elitism Rate: " << Constants::ELITISM_RATE << "\n";
    results_out << "Convergence Allowance: " << Constants::CONVERGENCE_ALLOWENCE << "\n";
    results_out << "Address Trace Size: " << address_vector.size() << "\n";
    results_out << "******************************************************************\n\n";


    // --- Experiment Loop ---
    std::vector<int> fitness_results;
    std::vector<int> generation_results;
    std::vector<double> time_results_ms;

    fitness_results.reserve(Constants::NUM_EXECUTIONS);
    generation_results.reserve(Constants::NUM_EXECUTIONS);
    time_results_ms.reserve(Constants::NUM_EXECUTIONS);

    for (int i = 0; i < Constants::NUM_EXECUTIONS; ++i) {
        std::string run_name = "GA" + std::to_string(i + 1);
        std::cout << "--- Starting Run " << (i + 1) << "/" << Constants::NUM_EXECUTIONS << " (" << run_name << ") ---" << std::endl;
        results_out << "--- Run: " << run_name << " ---\n";

        try {
            // Instantiate GA
            GeneticAlgorithm ga(run_name, address_vector, crossover_type);

            // Time the execution
            auto start_time = std::chrono::high_resolution_clock::now();
            ga.run(); // Execute the algorithm
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration_ms = end_time - start_time;

            // Get results from GA
            const Genome& best_genome = ga.getBestGenome();
            int final_fitness = best_genome.Fitness; // Or ga.getBestFitness()
            int final_generation = ga.getCurrentGeneration(); // This includes convergence allowance if converged early

            // Store results
            fitness_results.push_back(final_fitness);
            generation_results.push_back(final_generation);
            time_results_ms.push_back(duration_ms.count());

            // Output individual run results
            std::cout << run_name << " Complete! Fitness: " << final_fitness
                      << ", Generations: " << final_generation
                      << ", Time: " << duration_ms.count() << " ms\n";
            results_out << run_name << " Fitness (Evictions): " << final_fitness << "\n";
            results_out << run_name << " Generations Run: " << final_generation << "\n";
            results_out << run_name << " Execution Time (ms): " << duration_ms.count() << "\n";

             // --- Write GA Solution File ---
             std::string ga_solution_file_path = ga_solution_dir + "/GA_Solution - " + run_name + ".txt";
             std::ofstream ga_solution_out(ga_solution_file_path);
             if (ga_solution_out) {
                 ga_solution_out << "******* Solution - " << run_name << " (" << crossover_type << ") *******\n";
                 ga_solution_out << ga.displaySolutionInfo(); // Use the GA's method
                 ga_solution_out << "******************************************\n";
                 ga_solution_out.close();
             } else {
                 std::cerr << "ERROR: Could not open GA solution file: " << ga_solution_file_path << std::endl;
             }

            // --- Run and Write Cache Simulation ---
             LRUCache::simulateCache(run_name, crossover_type, best_genome.Chromosome, address_vector);

        } catch (const std::exception& e) {
             std::cerr << "ERROR during GA run " << run_name << ": " << e.what() << std::endl;
             results_out << run_name << " encountered an error: " << e.what() << "\n";
             // Decide if you want to skip this run or abort entirely
        }
         results_out << "---------------------\n";

    } // End of experiment loop

    // --- Calculate Summary Statistics ---
    if (!fitness_results.empty()) {
        double total_fitness = std::accumulate(fitness_results.begin(), fitness_results.end(), 0.0);
        double total_generations = std::accumulate(generation_results.begin(), generation_results.end(), 0.0);
        double total_time = std::accumulate(time_results_ms.begin(), time_results_ms.end(), 0.0);

        auto [min_fit_it, max_fit_it] = std::minmax_element(fitness_results.begin(), fitness_results.end());
        auto [min_gen_it, max_gen_it] = std::minmax_element(generation_results.begin(), generation_results.end());
        auto [min_time_it, max_time_it] = std::minmax_element(time_results_ms.begin(), time_results_ms.end());

        double avg_fitness = total_fitness / fitness_results.size();
        double avg_generations = total_generations / generation_results.size();
        double avg_time = total_time / time_results_ms.size();

        // --- Output Summary ---
        std::cout << "\n===== Experiment Summary for " << crossover_type << " =====\n";
        std::cout << std::fixed << std::setprecision(2); // Format output
        std::cout << "Fitness (Evictions): Min=" << *min_fit_it << ", Max=" << *max_fit_it << ", Avg=" << avg_fitness << "\n";
        std::cout << "Generations Run:     Min=" << *min_gen_it << ", Max=" << *max_gen_it << ", Avg=" << avg_generations << "\n";
        std::cout << "Execution Time (ms): Min=" << *min_time_it << ", Max=" << *max_time_it << ", Avg=" << avg_time << "\n";
        std::cout << "======================================\n";

        results_out << "\n********** Summary Statistics **********\n";
        results_out << std::fixed << std::setprecision(2);
        results_out << "Fitness (Evictions) - Min: " << *min_fit_it << "\n";
        results_out << "Fitness (Evictions) - Max: " << *max_fit_it << "\n";
        results_out << "Fitness (Evictions) - Avg: " << avg_fitness << "\n";
        results_out << "Generations Run     - Min: " << *min_gen_it << "\n";
        results_out << "Generations Run     - Max: " << *max_gen_it << "\n";
        results_out << "Generations Run     - Avg: " << avg_generations << "\n";
        results_out << "Execution Time (ms) - Min: " << *min_time_it << "\n";
        results_out << "Execution Time (ms) - Max: " << *max_time_it << "\n";
        results_out << "Execution Time (ms) - Avg: " << avg_time << "\n";
        results_out << "**************************************\n";
    } else {
         std::cout << "No successful runs completed for " << crossover_type << ".\n";
         results_out << "No successful runs completed.\n";
    }
    results_out.close();
}


int main(int argc, char* argv[]) {
    // TODO: Add command-line argument parsing (e.g., using cxxopts or similar)
    // to allow overriding crossover types, num_executions, output dirs etc.
    std::vector<std::string> crossover_types = {"UX", "2PX", "SX", "BAX"}; // Types to test

    // Seed the random number generator ONCE
    Utils::initialize_random_seed();

    // Generate the common address vector for all experiments in this run
    std::cout << "Generating address vector...\n";
    std::vector<std::bitset<Constants::ADDRESS_SIZE>> address_vector = Utils::randAddressVector();
    // Utils::displayAddressVector(address_vector); // Optional: display for debugging

    // Run experiments for each specified crossover type
    for (const std::string& co_type : crossover_types) {
        run_experiment(co_type, address_vector);
    }

    std::cout << "\nAll experiments complete." << std::endl;
    return 0;
}
