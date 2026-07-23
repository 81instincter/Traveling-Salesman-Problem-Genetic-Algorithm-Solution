#include "tsp_genetic_algorithm.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::pair;
using std::vector;

// ---------------------------------------------------------------------------
// Constructors
// ---------------------------------------------------------------------------
TSPGeneticAlgorithm::TSPGeneticAlgorithm()
    : numCities(10), populationSize(10), numberOfGenerations(100),
      crossoverRate(0.8), mutationRate(0.2) {}

TSPGeneticAlgorithm::TSPGeneticAlgorithm(int nc, double cr, double mr)
    : numCities(nc),
      populationSize(10 * nc),
      numberOfGenerations(100 * nc),
      crossoverRate(cr),
      mutationRate(mr) {}

// ---------------------------------------------------------------------------
// Setters
// ---------------------------------------------------------------------------
void TSPGeneticAlgorithm::setNumCities(int n) { numCities = n; }

void TSPGeneticAlgorithm::setPopulationSize(int n) { populationSize = n * 10; }

void TSPGeneticAlgorithm::setNumberOfGenerations(int n) {
    numberOfGenerations = n * 100;
}

void TSPGeneticAlgorithm::setCrossoverRate(double cr) { crossoverRate = cr; }

void TSPGeneticAlgorithm::setMutationRate(double mr) { mutationRate = mr; }

void TSPGeneticAlgorithm::setDistanceMatrix(
    const vector<vector<double>>& m) {
    distanceMatrix = m;
}

// ---------------------------------------------------------------------------
// Getters
// ---------------------------------------------------------------------------
int TSPGeneticAlgorithm::getNumCities() const { return numCities; }

int TSPGeneticAlgorithm::getPopulationSize() const { return populationSize; }

int TSPGeneticAlgorithm::getNumberOfGenerations() const {
    return numberOfGenerations;
}

double TSPGeneticAlgorithm::getCrossoverRate() const { return crossoverRate; }

double TSPGeneticAlgorithm::getMutationRate() const { return mutationRate; }

const vector<vector<double>>& TSPGeneticAlgorithm::getDistanceMatrix() const {
    return distanceMatrix;
}

// ---------------------------------------------------------------------------
// Initial population
// ---------------------------------------------------------------------------
vector<vector<int>> TSPGeneticAlgorithm::initialPopulationGenerator(
    int num_nodes, int population_size) {
    vector<int> cityNodes(num_nodes);
    std::iota(cityNodes.begin(), cityNodes.end(), 0);

    vector<vector<int>> population;
    population.reserve(population_size);

    std::random_device rd;
    std::mt19937 g(rd());

    for (int i = 0; i < population_size; ++i) {
        std::shuffle(cityNodes.begin(), cityNodes.end(), g);
        population.push_back(cityNodes);
    }

    numCities = num_nodes;
    populationSize = population_size;
    return population;
}

void TSPGeneticAlgorithm::populationPrinter(
    const vector<vector<int>>& pop) const {
    for (size_t i = 0; i < pop.size(); ++i) {
        cout << "Path " << (i + 1) << ": ";
        for (int city : pop[i]) {
            cout << city << " ";
        }
        cout << endl;
    }
}

// ---------------------------------------------------------------------------
// Fitness – higher value means shorter tour
// ---------------------------------------------------------------------------
double TSPGeneticAlgorithm::fitness(
    const vector<int>& chromosome,
    const vector<vector<double>>& distanceMatrix) const {
    if (chromosome.empty() || chromosome.size() < 2) {
        return 0.0;
    }

    double total = 0.0;
    const size_t n = chromosome.size();

    for (size_t i = 0; i < n - 1; ++i) {
        total += distanceMatrix[chromosome[i]][chromosome[i + 1]];
    }
    // Close the cycle
    total += distanceMatrix[chromosome.back()][chromosome.front()];

    if (total <= 1e-12) {
        return 1e9;
    }
    return 1000.0 / total;
}

// ---------------------------------------------------------------------------
// Tournament selection
// ---------------------------------------------------------------------------
vector<int> TSPGeneticAlgorithm::tournament(
    const vector<vector<int>>& T, int k) const {
    if (T.empty() || k <= 0) {
        return {};
    }

    k = std::min(k, static_cast<int>(T.size()));

    vector<int> best = T[0];
    double bestFitness = fitness(best, distanceMatrix);

    for (int i = 1; i < k; ++i) {
        double fit = fitness(T[i], distanceMatrix);
        if (fit > bestFitness) {
            bestFitness = fit;
            best = T[i];
        }
    }
    return best;
}

vector<vector<int>> TSPGeneticAlgorithm::tournamentSelection(
    const vector<vector<int>>& P, int k, int n) const {
    vector<vector<int>> selected;
    selected.reserve(n);

    for (int i = 0; i < n; ++i) {
        vector<vector<int>> tournamentGroup;
        tournamentGroup.reserve(k);
        for (int j = 0; j < k; ++j) {
            tournamentGroup.push_back(P[rand() % P.size()]);
        }
        selected.push_back(tournament(tournamentGroup, k));
    }
    return selected;
}

// ---------------------------------------------------------------------------
// Partially Mapped Crossover (PMX)
// ---------------------------------------------------------------------------
vector<vector<int>> TSPGeneticAlgorithm::pmxCrossover(
    const vector<int>& father, const vector<int>& mother) const {
    if (father.size() != mother.size()) {
        return {vector<int>(distanceMatrix.size(), 0),
                vector<int>(distanceMatrix.size(), 0)};
    }

    const int n = static_cast<int>(father.size());
    if (n < 2) {
        return {father, mother};
    }

    // Random crossover points
    int left = rand() % n;
    int right = rand() % n;
    if (left == right) {
        if (left + 1 < n)
            right = left + 1;
        else
            left = left - 1;
    }
    if (left > right) std::swap(left, right);

    vector<vector<int>> offspring(2, vector<int>(n));
    offspring[0] = father;
    offspring[1] = mother;

    std::unordered_map<int, int> map0, map1;

    for (int i = left; i < right; ++i) {
        offspring[0][i] = mother[i];
        map0[mother[i]] = father[i];

        offspring[1][i] = father[i];
        map1[father[i]] = mother[i];
    }

    // Resolve conflicts outside the crossover segment
    auto resolve = [](vector<int>& child, const std::unordered_map<int, int>& mapping,
                      int start, int end) {
        for (int i = start; i < end; ++i) {
            while (mapping.count(child[i])) {
                child[i] = mapping.at(child[i]);
            }
        }
    };

    resolve(offspring[0], map0, 0, left);
    resolve(offspring[0], map0, right, n);
    resolve(offspring[1], map1, 0, left);
    resolve(offspring[1], map1, right, n);

    return offspring;
}

vector<vector<int>> TSPGeneticAlgorithm::pmxCrossoverBreeding(
    const vector<vector<int>>& pop, double crossoverRate) const {
    if (crossoverRate < 0.0 || crossoverRate > 1.0) {
        return {};
    }

    vector<vector<int>> newPop;
    const int n = static_cast<int>(pop.size());
    int pairsCount = (n % 2 == 0) ? n : n - 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i + 1 < pairsCount; i += 2) {
        const vector<int>& father = pop[i];
        const vector<int>& mother = pop[i + 1];

        if (dist(gen) <= crossoverRate) {
            auto children = pmxCrossover(father, mother);
            newPop.push_back(children[0]);
            newPop.push_back(children[1]);
        } else {
            newPop.push_back(father);
            newPop.push_back(mother);
        }
    }

    if (n % 2 == 1) {
        newPop.push_back(pop.back());
    }

    if (newPop.size() > pop.size()) {
        newPop.resize(pop.size());
    }
    return newPop;
}

vector<vector<int>> TSPGeneticAlgorithm::pmxCrossoverBreedingFitness(
    const vector<vector<int>>& pop, double crossoverRate) const {
    if (pop.empty()) return {};

    // Rank by fitness (descending)
    vector<pair<double, int>> ranked;
    ranked.reserve(pop.size());
    for (size_t i = 0; i < pop.size(); ++i) {
        ranked.emplace_back(fitness(pop[i], distanceMatrix), static_cast<int>(i));
    }
    std::sort(ranked.begin(), ranked.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    // Keep top half
    const int numTop = std::max(2, static_cast<int>(pop.size()) / 2);
    vector<vector<int>> topPop;
    topPop.reserve(numTop);
    for (int i = 0; i < numTop; ++i) {
        topPop.push_back(pop[ranked[i].second]);
    }

    // Breed among the elite
    vector<vector<int>> newPop;
    const int n = static_cast<int>(topPop.size());
    int pairsCount = (n % 2 == 0) ? n : n - 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i + 1 < pairsCount; i += 2) {
        if (dist(gen) <= crossoverRate) {
            auto children = pmxCrossover(topPop[i], topPop[i + 1]);
            newPop.push_back(std::move(children[0]));
            newPop.push_back(std::move(children[1]));
        } else {
            newPop.push_back(topPop[i]);
            newPop.push_back(topPop[i + 1]);
        }
    }

    if (n % 2 == 1) {
        newPop.push_back(topPop.back());
    }

    if (newPop.size() > pop.size()) {
        newPop.resize(pop.size());
    }
    return newPop;
}

// ---------------------------------------------------------------------------
// Mutation
// ---------------------------------------------------------------------------
void TSPGeneticAlgorithm::swapMutation(vector<int>& chromosome,
                                       double mutationRate) const {
    if (mutationRate < 0.0 || mutationRate > 1.0 || chromosome.size() < 2) {
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> realDist(0.0, 1.0);

    if (realDist(gen) > mutationRate) {
        return;  // no mutation this time
    }

    std::uniform_int_distribution<int> idxDist(0, static_cast<int>(chromosome.size()) - 1);
    int i = idxDist(gen);
    int j;
    do {
        j = idxDist(gen);
    } while (j == i);

    std::swap(chromosome[i], chromosome[j]);
}

// ---------------------------------------------------------------------------
// Replacement
// ---------------------------------------------------------------------------
void TSPGeneticAlgorithm::solutionReplacer(
    vector<vector<int>>& originalPopulation,
    const vector<vector<int>>& solutionMatrix) const {
    originalPopulation = solutionMatrix;
}