#ifndef GEN_ALG_H
#define GEN_ALG_H

// Includes
#include <bits/stdc++.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <time.h>
#include <bitset>
#include <random>
// C program to create a folder
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <filesystem>
#include <typeinfo>

using namespace std;

// Initial Definitions
#define POP_SIZE 100
#define ELITISM_PERCENTAGE 5 // What top percentage of the population should be carried from generation to generation
#define MUTATION_CHANCE 5 // As a percentage chance
#define MAX_GEN 400 // The max allowed generations
#define SOLUTION_SIZE 300
#define ADDRESS_SIZE 32
#define CONVERGENCE_ALLOWENCE 25 // The # of generations with no fitness change to declare as converged
#define NUM_EXECUTIONS 20
#define SEG_X_CHANCE 40 // chance to make a new crossover point for segmented crossover
const string genes = "10";
/***********************************************************************************************************/
class Genome {

    public:
        bitset<SOLUTION_SIZE> Chromosome;
        vector<bitset<ADDRESS_SIZE>> addressVector;
        int Fitness;
        int cal_fitness();
        Genome(bitset<SOLUTION_SIZE> chromosome, vector<bitset<ADDRESS_SIZE>> addressVector): Chromosome(chromosome), addressVector(addressVector), Fitness(cal_fitness()){};
        pair<Genome, Genome> UX(Genome mate);
        pair<Genome, Genome> TWOPX(Genome mate);
        pair<Genome, Genome> SX(Genome mate);
        pair<Genome, Genome> BAX(Genome mate);
        bool operator<(const Genome& GenomeB);
};
/***********************************************************************************************************/

/***********************************************************************************************************/
class GenAlg {

    public:
        vector<bitset<ADDRESS_SIZE>> addressVector;
        string crossoverType;
        int currentGen;
        int solFitness;
        string Name;
        vector<Genome> population;
        bool solutionFound;
        ofstream output;
        GenAlg(string name, vector<bitset<ADDRESS_SIZE>> addressVector, string crossoverType): Name(name), addressVector(addressVector), crossoverType(crossoverType){};
        void run();
        void runWithOF();
        string displayPop();
        string displaySolution();
        int getFitness(){return solFitness;};
        int getGen(){return currentGen;};
};
/***********************************************************************************************************/

/***********************************************************************************************************/
class Tools {

    public:
        static int randRange(int min, int max);
        static char randChar();
        static string newGenome();
        static string addressToHex(bitset<ADDRESS_SIZE> address);
        static void displayAddressVector(vector<bitset<ADDRESS_SIZE>> addressVector);
        static vector<bitset<ADDRESS_SIZE>> randAddressVector();
        static void runGA(vector<bitset<ADDRESS_SIZE>> addressVector, string crossoverType, string output);
};

/*******************************************************************************************************************************/
class LRUSet {
    // store keys of cache
    list<bitset<ADDRESS_SIZE>> set;
 
    // store references of key in cache
    unordered_map<bitset<ADDRESS_SIZE>, list<bitset<ADDRESS_SIZE>>::iterator> cacheMapping;
    int size; // maximum capacity of cache
 
public:
    LRUSet();
    bool refer(bitset<ADDRESS_SIZE>);
    string display();
};
/***********************************************************************************************************/

/*******************************************************************************************************************************/
class LRUCache {
    
    //int size; // maximum capacity of cache
    LRUSet setArray[8];
    int evictionCount = 0;
 
public:
    int getEvictionCount();
    LRUCache();
    string display();
    void index(int, bitset<ADDRESS_SIZE>);
    static void simulateCache(string, string, bitset<SOLUTION_SIZE>, vector<bitset<ADDRESS_SIZE>>);
};
/*******************************************************************************************************************************/


#endif // GEN_ALG_H