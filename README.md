# Genetic Algorithms - Optimizing Cache Indexing

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![Language: C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/) [![Build: CMake](https://img.shields.io/badge/Build-CMake-green.svg)](https://cmake.org/)

## Overview & Goal

This C++ project explores using a Genetic Algorithm (GA) to optimize **static cache set indexing functions**. The goal is to find an address-to-set mapping that minimizes cache conflicts (measured by evictions) for a given workload trace, evaluated via an **LRU (Least Recently Used) cache simulation**.

Developed as a university project, it served as a practical exercise in implementing GAs and applying them to a computer systems problem, focusing on the _offline design_ of cache indexing rather than real-time operation.

**Clarification:** The GA optimizes the _indexing policy_ (how addresses map to sets). The cache simulation uses a fixed _LRU replacement policy_ to evaluate the fitness of these indexing schemes.

## Key Features

- **Genetic Algorithm:** Implements standard GA operators (Selection, UX, 2PX, SX, BAX Crossovers, Mutation) to evolve indexing schemes.
- **LRU Cache Simulator:** Models a set-associative cache to evaluate the fitness (eviction count) of GA solutions.
- **Experiment Framework:** Compares different GA crossover strategies across multiple runs.
- **Modern C++:** Written in C++17 using STL containers, `<random>`, `<filesystem>`, etc.
- **CMake Build:** Provides a cross-platform build process.

## Technical Approach

- **GA Representation:** A binary chromosome defines how address bits are mapped to cache set index bits.
- **Fitness Evaluation:** Fitness = Total evictions from the LRU cache simulation using the chromosome's indexing scheme. Lower is better.
- **Simulation Context:** The fixed LRU policy is used purely to evaluate the effectiveness of the GA-proposed _static indexing_ schemes.

## Setup and Usage

1.  **Prerequisites:** C++17 compiler, CMake (>= 3.14).
2.  **Clone:** `git clone https://github.com/calebkey121/genetic-algorithm.git && cd genetic-algorithm`
3.  **Build:** `mkdir build && cd build && cmake .. && make`
4.  **Run:** `./GeneticAlgorithmCacheOptimization`
5.  **Output:** Results/logs are saved to `Results/`, `*_GA_Solution/`, `*_Simulation/` directories.

## Limitations

- **Offline Design Tool:** The GA is for exploring _static_ indexing schemes offline, not real-time cache operations.
- **Workload Dependent:** Results are specific to the simulated address trace used for evaluation.
- **Simplified Simulation:** Does not model all advanced cache features.

## Key Learnings & Transferable Skills

This project demonstrates:

- **Optimization Algorithms:** Genetic Algorithm implementation.
- **C++ Development:** Modern C++ programming, OOP, STL.
- **Simulation:** Building simulators for system evaluation.
- **Problem Modeling:** Applying AI techniques to system problems.
- **Experimentation:** Comparing algorithmic approaches.
- **Software Engineering:** Modular code, build systems (CMake).

## License

MIT License - see [LICENSE](LICENSE) file.
