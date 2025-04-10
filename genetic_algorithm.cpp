#include "genetic_algorithm.h"

using namespace std;

/***********************************************************************************************************/
/********************************************************************************************
Uniform Crossover
Mates: This crossover requires two parents
Pick Order: A binary number, with length equaling the number of genes, will direct each gene
            to its designated offspring; with '0' representing the first mate's gene, and '1' 
            representing the other
Offspring: This operation produces 2 offspring, one following the exact pick order, and the
           other following the exact opposite of the pick order
Example:
Mates: 'Hello' | 'Piano'
Pick Order: 11001
Offspring: 'Pillo' | 'Heano'
*********************************************************************************************/
pair<Genome, Genome> Genome::UX(Genome mate){
    // Pseudo: Determine the reigons for inheritance
    //         Generate a random number to determine crossover
    //         Assign offspring gene based on probability - parent A, B, or mutate
               
    pair<bitset<SOLUTION_SIZE>, bitset<SOLUTION_SIZE>> offspringBitsets;

    // Reigons for inheritance
    float parentRangeA = (1 - MUTATION_CHANCE/100) / 2; // this parent (A)
    float parentRangeB = 1 - MUTATION_CHANCE/100; // mate (B)

    for (int i = 0; i < SOLUTION_SIZE; i++){
        float p = Tools::randRange(0, 100) / 100;

        if (p < parentRangeA){
            offspringBitsets.first[i] = Chromosome[i];
            offspringBitsets.second[i] = mate.Chromosome[i];
        }
        else if (p < parentRangeB){
            offspringBitsets.first[i] = mate.Chromosome[i];
            offspringBitsets.second[i] = Chromosome[i];
        }
        else { // Mutate
            offspringBitsets.first[i] = Tools::randChar();
            offspringBitsets.second[i] = Tools::randChar();
        }
    }
    pair<Genome, Genome> offspring = make_pair(Genome(offspringBitsets.first, addressVector), Genome(offspringBitsets.second, addressVector));

    return offspring;
};

/********************************************************************************************
Crossover Operation - This example will use 'Uniform Crossover'
Mates: This crossover requires two parents
Pick Order: 
Offspring: 
*********************************************************************************************/
pair<Genome, Genome> Genome::TWOPX(Genome mate){
    // https://stackoverflow.com/questions/7145583/two-point-crossover-operation
    // Pseudo: Determine two distinct random gene locations
    //         Generate a random number to determine crossover
    //         Assign offspring gene based on probability - parent A, B, or mutate
               
    pair<bitset<SOLUTION_SIZE>, bitset<SOLUTION_SIZE>> offspringBitsets;
    float mutationChance = 1 - MUTATION_CHANCE/100;

    // Generate two distinct random locations, making sure that B > A
    int randLocationA = rand() % SOLUTION_SIZE;
    int randLocationB = rand() % SOLUTION_SIZE;
    while (randLocationA == randLocationB or randLocationA > randLocationB) {
        randLocationA = rand() % SOLUTION_SIZE;
        randLocationB = rand() % SOLUTION_SIZE;
    }
    for (int i = 0; i < randLocationA; i++){
        float p = Tools::randRange(0, 100) / 100;
        if (p < mutationChance){
            offspringBitsets.first[i] = Chromosome[i];
            offspringBitsets.second[i] = mate.Chromosome[i];
        }
        else {
            offspringBitsets.first[i] = Tools::randChar();
            offspringBitsets.second[i] = Tools::randChar();
        }
    }
    for (int i = randLocationA; i < randLocationB; i++){
        float p = Tools::randRange(0, 100) / 100;
        if (p < mutationChance){
            offspringBitsets.second[i] = Chromosome[i];
            offspringBitsets.first[i] = mate.Chromosome[i];
        }
        else {
            offspringBitsets.first[i] = Tools::randChar();
            offspringBitsets.second[i] = Tools::randChar();
        }
    }
    for (int i = randLocationB; i < SOLUTION_SIZE; i++){
        float p = Tools::randRange(0, 100) / 100;
        if (p < mutationChance){
            offspringBitsets.first[i] = Chromosome[i];
            offspringBitsets.second[i] = mate.Chromosome[i];
        }
        else {
            offspringBitsets.first[i] = Tools::randChar();
            offspringBitsets.second[i] = Tools::randChar();
        }
    }

    pair<Genome, Genome> offspring = make_pair(Genome(offspringBitsets.first, addressVector), Genome(offspringBitsets.second, addressVector));

    return offspring;
};

/********************************************************************************************
Crossover Operation - This example will use 'Uniform Crossover'
Mates: This crossover requires two parents
Pick Order: 
Offspring: 
*********************************************************************************************/
pair<Genome, Genome> Genome::SX(Genome mate){
    // 
    // Pseudo: 
               
    pair<bitset<SOLUTION_SIZE>, bitset<SOLUTION_SIZE>> offspringBitsets;

    // Reigons for inheritance
    float mutationChance = 1 - MUTATION_CHANCE/100;
    float segmentationChance = mutationChance - (1 - SEG_X_CHANCE/100); // this parent (A)

    bool switchParents = false;
    for (int i = 0; i < SOLUTION_SIZE; i++){
        float p = Tools::randRange(0, 100) / 100;

        if (p < segmentationChance){
            if (switchParents){
                offspringBitsets.second[i] = mate.Chromosome[i];
                offspringBitsets.first[i] = Chromosome[i];
            }
            else {
                offspringBitsets.first[i] = mate.Chromosome[i];
                offspringBitsets.second[i] = Chromosome[i];
            }
        }
        else if (p < mutationChance){ // Segmentation
            switchParents = !switchParents;
            if (switchParents){
                offspringBitsets.second[i] = mate.Chromosome[i];
                offspringBitsets.first[i] = Chromosome[i];
            }
            else {
                offspringBitsets.first[i] = mate.Chromosome[i];
                offspringBitsets.second[i] = Chromosome[i];
            }
        }
        else { // Mutate
            offspringBitsets.first[i] = Tools::randChar();
            offspringBitsets.second[i] = Tools::randChar();
        }
    }

    pair<Genome, Genome> offspring = make_pair(Genome(offspringBitsets.first, addressVector), Genome(offspringBitsets.second, addressVector));

    return offspring;
};

/********************************************************************************************
Crossover Operation - This example will use 'Uniform Crossover'
Mates: This crossover requires two parents
Pick Order: 
Offspring: 
*********************************************************************************************/
pair<Genome, Genome> Genome::BAX(Genome mate){
    // 
    // Pseudo: 

    int i = SOLUTION_SIZE - 1;
    bool parentABit = Chromosome[i];
    bool parentBBit = mate.Chromosome[i];
    if (parentABit != parentBBit){
        float p = Tools::randRange(0, 100) / 100;
        if (p < 0.5){
            return this->UX(mate);
        }
        else {
            return this->TWOPX(mate);
        }
    }
    else if (parentABit == true){
        return this->UX(mate);
    }
    else {
        return this->TWOPX(mate);
    }
};

// Fittness score = the number of characters in string which differ from target
// The lower, the better score
int Genome::cal_fitness(){
    int fitness = 0;
    
    LRUCache cache;
    bitset<3> index;
    for (int i = 0; i < 100; i++){
        index[0] = Chromosome[i * 3];
        index[1] = Chromosome[(i * 3) + 1];
        index[2] = Chromosome[(i * 3) + 2];
        cache.index((int)index.to_ulong(), addressVector.at(i));
    }
 
    fitness = cache.getEvictionCount();

    return fitness;    
};

// Overloading < operator - will help when we sort the population by fitness
bool Genome::operator<(const Genome& GenomeB)
{
    return Fitness < GenomeB.Fitness;
};
/***********************************************************************************************************/

/***********************************************************************************************************/
void GenAlg::runWithOF(){

    output.open(crossoverType + "_GA_Solution/GA Solution - " + Name + ".txt");
    output << "*******Solution - " + Name + "*******" << endl;

    currentGen = 0;
    solutionFound = false;
    // Initial Population
    for (int i = 0; i < POP_SIZE; i++){
        string genomeString = Tools::newGenome();
        bitset<SOLUTION_SIZE> genome(genomeString);
        population.push_back(Genome(genome, addressVector));
    }

    bool convergence = false;
    int convergenceCounter = 0;
    int previousFitness = 0;
    while(!solutionFound and convergence == false){
        output << "Fitness: " + to_string(population.at(0).Fitness) + "\n";
        output << "ConvergenceCounter: " + to_string(convergenceCounter) + "\n";
        sort(population.begin(), population.end());

        // check if target has been found
        if (population.front().Fitness == 0 || currentGen == MAX_GEN){
            solutionFound = true;
            break;
        }

        // Case - No Solution
        vector<Genome> newPopulation;
        // Calculate Elitism
        int elitismRange = (ELITISM_PERCENTAGE * POP_SIZE) / 100;
        for(int i = 0; i < elitismRange; i++){
            newPopulation.push_back(population.at(i));
        }

        // Selection Process - Random for now
        int remainingRange = POP_SIZE - elitismRange; // remaining portion of pop
        for(int i = 0; i < remainingRange; i+=2){ // Each iteration generates two offspring
            Genome parentA = population.at(Tools::randRange(0, 50));
            Genome parentB = population.at(Tools::randRange(0, 50));
            pair<Genome, Genome> offspring = parentA.UX(parentB);

            if (crossoverType == "UX") offspring = parentA.UX(parentB);
            else if (crossoverType == "2PX") offspring = parentA.TWOPX(parentB);
            else if (crossoverType == "SX") offspring = parentA.SX(parentB);
            else if (crossoverType == "BAX") offspring = parentA.BAX(parentB);
            else {
                printf("ERROR: No valid crossover selected");
                exit(1);
            }

            newPopulation.push_back(offspring.first);
            if (newPopulation.size() == POP_SIZE) break; // check if second offspring is necessary
            newPopulation.push_back(offspring.second);
        }

        // Check for convergence
        if (previousFitness == population.at(0).Fitness) convergenceCounter++;
        else {
            convergenceCounter = 0;
        }
        previousFitness = population.at(0).Fitness;

        if (convergenceCounter == CONVERGENCE_ALLOWENCE) convergence = true;

        // Replace old population
        population = newPopulation;

        // Advance Generation
        currentGen++;

    }

    // Write solution to output file
    output << displaySolution();
    output << "*******Solution - " + Name + "*******" << endl;
    LRUCache::simulateCache(Name, crossoverType, population.at(0).Chromosome, population.at(0).addressVector);
    output.close();
    solFitness = population.at(0).Fitness;
};

void GenAlg::run(){

    currentGen = 0;
    solutionFound = false;
    // Initial Population
    for (int i = 0; i < POP_SIZE; i++){
        string genomeString = Tools::newGenome();
        bitset<SOLUTION_SIZE> genome(genomeString);
        population.push_back(Genome(genome, addressVector));
    }

    bool convergence = false;
    int convergenceCounter = 0;
    int previousFitness = 0;
    while(!solutionFound and convergence == false){
        sort(population.begin(), population.end());

        // check if target has been found
        if (population.front().Fitness == 0 || currentGen == MAX_GEN){
            solutionFound = true;
            break;
        }

        // Case - No Solution
        vector<Genome> newPopulation;
        // Calculate Elitism
        int elitismRange = (ELITISM_PERCENTAGE * POP_SIZE) / 100;
        for(int i = 0; i < elitismRange; i++){
            newPopulation.push_back(population.at(i));
        }

        // Selection Process - Random for now
        int remainingRange = POP_SIZE - elitismRange; // remaining portion of pop
        for(int i = 0; i < remainingRange; i+=2){ // Each iteration generates two offspring
            Genome parentA = population.at(Tools::randRange(0, 50));
            Genome parentB = population.at(Tools::randRange(0, 50));
            pair<Genome, Genome> offspring = parentA.UX(parentB);

            if (crossoverType == "UX") offspring = parentA.UX(parentB);
            else if (crossoverType == "2PX") offspring = parentA.TWOPX(parentB);
            else if (crossoverType == "SX") offspring = parentA.SX(parentB);
            else if (crossoverType == "BAX") offspring = parentA.BAX(parentB);
            else {
                printf("ERROR: No valid crossover selected");
                exit(1);
            }

            newPopulation.push_back(offspring.first);
            if (newPopulation.size() == POP_SIZE) break; // check if second offspring is necessary
            newPopulation.push_back(offspring.second);
        }


        // Check for convergence
        if (previousFitness == population.at(0).Fitness) convergenceCounter++;
        else {
            convergenceCounter = 0;
        }
        previousFitness = population.at(0).Fitness;

        if (convergenceCounter == CONVERGENCE_ALLOWENCE) convergence = true;

        // Replace old population
        population = newPopulation;

        // Advance Generation
        currentGen++;

    }
    solFitness = population.at(0).Fitness;
};

string GenAlg::displayPop(){
    string returnPop;
    returnPop += ("Generation: " + to_string(currentGen) + "\n");
    for (int i = 0; i < population.size(); i++){
        returnPop += (population.at(i).Chromosome.to_string() + "\n");
    }
    returnPop += ("Best Solution: " + population.at(0).Chromosome.to_string() + "\n");
    returnPop += ("Fitness: " + to_string(population.at(0).Fitness) + "\n");
    returnPop += ("**************************************************************\n");
    return returnPop;
};

string GenAlg::displaySolution(){
    string returnPop;
    returnPop += ("Generation: " + to_string(currentGen) + "\n");
    returnPop += ("Best Solution: " + population.at(0).Chromosome.to_string() + "\n");
    returnPop += ("Fitness: " + to_string(population.at(0).Fitness) + "\n");
    return returnPop;
};

/***********************************************************************************************************/

/***********************************************************************************************************/
int Tools::randRange(int min, int max){
    // Returns a random integer given a range
    
    int range = max - min + 1;
    return (rand() % range) + min;
};

char Tools::randChar(){
    // returns a random charater within the gene pool

    return (genes[randRange(0, genes.length() - 1)]);
};

string Tools::newGenome(){
    // Create a random new Genome

    string genome = "";
    for (int i = 0; i < SOLUTION_SIZE; i++){
        genome += randChar();
    }
    return genome;
};

string Tools::addressToHex(bitset<ADDRESS_SIZE> address){
    stringstream ss;
    bitset<4> bit_to_hex;
    for (int i = 0; i < (ADDRESS_SIZE / 4); i++){
        bit_to_hex[0] = address[i];
        bit_to_hex[1] = address[i + 1];
        bit_to_hex[2] = address[i + 2];
        bit_to_hex[3] = address[i + 3];
        ss << hex <<  bit_to_hex.to_ulong();
    }
    return ss.str();
}

void Tools::displayAddressVector(vector<bitset<ADDRESS_SIZE>> addressVector){
    cout << "<";
        for (int i = 0; i < addressVector.size(); i++){
            cout << addressToHex(addressVector.at(i)) << ", ";
        }
    cout << ">" << endl;
}

vector<bitset<ADDRESS_SIZE>> Tools::randAddressVector(){
    // Generating 100 random 32-bit Addresses
    srand(time(NULL));
    bitset<ADDRESS_SIZE> bits;
    double p = 0.5;
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution d(p);
    vector<bitset<ADDRESS_SIZE>> subVector;
    vector<bitset<ADDRESS_SIZE>> addressVector;
    for (int i = 0; i < 4; i++){
        while (subVector.size() != 25){
            for( int n = 0; n < ADDRESS_SIZE; ++n) {
                bits[n] = d(gen);
            }

            // This populates the vector with repeat addresses to simulate a cache line thatt likely calls an address multiple times
            int repeat = rand() % 10 + 1;

            if (repeat > (25 - subVector.size())){
                repeat = 25 - subVector.size();
            }
            
            for(int i = 0; i < repeat; i ++){
                subVector.push_back(bits);
            }

        }
        // duplicate addresses should be shuffled
        auto rng = default_random_engine {};
        shuffle(begin(subVector), end(subVector), rng);
        //Tools::displayAddressVector(subVector);
        for (int s = 0; s < 25; s++){
            addressVector.push_back(subVector.at(s));
        }
        subVector.clear();
    }

    return addressVector;
}

void Tools::runGA(vector<bitset<ADDRESS_SIZE>> addressVector, string crossoverType, string output){
    int GAFitness = 0;
    int GAGen = 0;
    float numExecutions = NUM_EXECUTIONS;
    string GAName;
    vector<int> fitness;
    vector<int> generations;
    ofstream results;

    // Delete Previous Directories
    // create new directory
    string folderName = crossoverType + "_GA_Solution";
    char dirName[folderName.length()+1];
    strcpy(dirName, folderName.c_str());
    mkdir(dirName);
    
    folderName = crossoverType + "_Simulation";
    strcpy(dirName, folderName.c_str());
    mkdir(dirName);
    
    folderName = "Results";
    strcpy(dirName, folderName.c_str());
    mkdir(dirName);


    results.open("Results/" + crossoverType + "_Results.txt");
    for(int i = 0; i < numExecutions; i++){
        results << "*******************" << endl;
        cout << "*******************" << endl;
        cout << crossoverType << endl;
        
        GAName = "GA" + to_string(i + 1);
        GenAlg newAlg(GAName, addressVector, crossoverType);

        cout << GAName << " is running..." << endl;
        if (output == "YES") newAlg.runWithOF();
        else newAlg.run();
        GAFitness = newAlg.getFitness();
        results << GAName << " # of Evictions: " << to_string(GAFitness) << endl;
        GAGen = newAlg.getGen()  - CONVERGENCE_ALLOWENCE;
        results << GAName << " # of Generations to Convergence: " << to_string(GAGen) << endl;
        cout << GAName << " Complete!" << endl << endl;

        fitness.push_back(GAFitness);
        generations.push_back(GAGen);
    }

    // Calculate Min, Max, Average
    int worstFitness, bestFitness, worstGen, bestGen;
    sort(fitness.begin(), fitness.end());
    sort(generations.begin(), generations.end());

    bestFitness = fitness.front();
    worstFitness = fitness.back();
    bestGen = generations.front();
    worstGen = generations.back();

    float totalFitness = accumulate(fitness.begin(), fitness.end(), 0.0);
    float totalGens = accumulate(generations.begin(), generations.end(), 0.0);

    float avgFitness = totalFitness / numExecutions;
    float avgGen = totalGens / numExecutions;

    results << "*******************" << endl;
    results << "Best Fitness: " << bestFitness << endl;
    results << "Worst Fitness: " << worstFitness << endl;
    results << "Average Fitness: " << avgFitness << endl;
    results << "Most # of Generation to Converge: " << worstGen << endl;
    results << "Least # of Generation to Converge: " << bestGen << endl;
    results << "Average # of Generations to Converge: " << avgGen << endl;
    results.close();
}
/***********************************************************************************************************/

/***********************************************************************************************************/
// Declare the size
LRUSet::LRUSet()
{
    size = 2;
}
 
// Refers key x with in the LRU cache | returns true if resulted in an eviction
bool LRUSet::refer(bitset<ADDRESS_SIZE> x)
{
    bool eviction = false;
    // not present in cache
    if (cacheMapping.find(x) == cacheMapping.end()) {
        // cache is full
        if (set.size() == size) { // EVICTION
            eviction = true;
            // delete least recently used element
            bitset<ADDRESS_SIZE> last = set.back();
 
            // Pops the last element
            set.pop_back();
 
            // Erase the last
            cacheMapping.erase(last);
        }
    }
 
    // present in cache
    else
        set.erase(cacheMapping[x]);
 
    // update reference
    set.push_front(x);
    cacheMapping[x] = set.begin();

    return eviction;
}
 
// Function to display contents of cache
string LRUSet::display()
{
 
    // Iterate in the deque and print
    // all the elements in it
    stringstream ss;

    if (set.size() == 0){
        ss << "| -------- | -------- | ";
    }
    else if (set.size() == 1){
        ss << "| " << Tools::addressToHex(set.front()) << " | -------- | ";
    }
    else{
        ss << "| " << Tools::addressToHex(set.front()) << " | " << Tools::addressToHex(set.back()) << " | ";
    }

    ss << endl;
    return ss.str();
}
/***********************************************************************************************************/

/***********************************************************************************************************/
LRUCache::LRUCache(){
    
}

int LRUCache::getEvictionCount(){
    return evictionCount;
}

string LRUCache::display(){
    stringstream ss;
    for (int i = 0; i < 8; i++) ss << setArray[i].display();
    ss << "Eviction Count: " << evictionCount << endl;
    return ss.str();
}

void LRUCache::index(int set, bitset<ADDRESS_SIZE> value){
    if (setArray[set].refer(value)){
        evictionCount++;
    }
}

void LRUCache::simulateCache(string name, string crossoverType, bitset<SOLUTION_SIZE> chromosome, vector<bitset<ADDRESS_SIZE>> addressVector){
    ofstream simulation;
    simulation.open(crossoverType + "_Simulation/Simulation - " + name + ".txt");
    
    LRUCache cache;
    
    bitset<3> index;
    for (int i = 0; i < 100; i++){
        index[0] = chromosome[(i * 3)];
        index[1] = chromosome[(i * 3) + 1];
        index[2] = chromosome[(i * 3) + 2];
        cache.index((int)index.to_ulong(), addressVector.at(i));
        simulation << "Index: " << (int)index.to_ulong() << " (" << index.to_string() << ")";
        simulation << " | Address No. " << i + 1 << " | Address: " << Tools::addressToHex(addressVector.at(i)) << endl;
        simulation << cache.display();
        simulation << "********************************" << endl;
    }

    simulation.close();
}
/***********************************************************************************************************/

/***********************************************************************************************************/

int main() {
    // Generating 100 random 32-bit Addresses
    vector<bitset<ADDRESS_SIZE>> addressVector = Tools::randAddressVector();
    /*
    UX - Uniform Crossover
    2PX - Two Point Crossover
    SX - Segmented Crossover
    */

    // AV, crossoverType, Output - YES or NO?
    Tools::runGA(addressVector, "UX", "YES");
    Tools::runGA(addressVector, "2PX", "YES");
    Tools::runGA(addressVector, "SX", "YES");
    Tools::runGA(addressVector, "BAX", "YES");

    return 0;
}