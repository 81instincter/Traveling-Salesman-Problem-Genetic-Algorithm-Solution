#ifndef TSP_GENETIC_ALGORITHM_HPP
#define TSP_GENETIC_ALGORITHM_HPP

#include <vector>
#include <iostream>

/**
 * @class TSPGeneticAlgorithm
 * @brief Genetic Algorithm solver for the Traveling Salesman Problem (TSP).
 *
 * Uses tournament selection, Partially-Mapped Crossover (PMX), and swap mutation.
 * Population is iteratively reduced by selecting fitter individuals until a small
 * elite set remains. Designed for educational demonstration and comparison with
 * exhaustive (brute-force) search on small instances.
 */
class TSPGeneticAlgorithm {
private:
    std::vector<std::vector<double>> distanceMatrix;
    int numCities;

    // GA Parameters
    int populationSize;
    int numberOfGenerations;
    double crossoverRate;
    double mutationRate;

public:
    // Constructors
    TSPGeneticAlgorithm();
    TSPGeneticAlgorithm(int nc, double cr, double mr);

    // Setter methods
    void setNumCities(int n);
    void setPopulationSize(int n);
    void setNumberOfGenerations(int n);
    void setCrossoverRate(double cr);
    void setMutationRate(double mr);
    void setDistanceMatrix(const std::vector<std::vector<double>>& m);

    // Getter methods
    int getNumCities() const;
    int getPopulationSize() const;
    int getNumberOfGenerations() const;
    double getCrossoverRate() const;
    double getMutationRate() const;
    const std::vector<std::vector<double>>& getDistanceMatrix() const;

    // Initial population
    std::vector<std::vector<int>> initialPopulationGenerator(int num_nodes, int population_size);
    void populationPrinter(const std::vector<std::vector<int>>& pop) const;

    // Fitness (higher is better = shorter tour)
    double fitness(const std::vector<int>& chromosome,
                   const std::vector<std::vector<double>>& distanceMatrix) const;

    // Selection
    std::vector<int> tournament(const std::vector<std::vector<int>>& T, int k) const;
    std::vector<std::vector<int>> tournamentSelection(const std::vector<std::vector<int>>& P,
                                                       int k, int n) const;

    // Partially Mapped Crossover
    std::vector<std::vector<int>> pmxCrossover(const std::vector<int>& father,
                                               const std::vector<int>& mother) const;
    std::vector<std::vector<int>> pmxCrossoverBreeding(const std::vector<std::vector<int>>& pop,
                                                       double crossoverRate) const;
    std::vector<std::vector<int>> pmxCrossoverBreedingFitness(const std::vector<std::vector<int>>& pop,
                                                              double crossoverRate) const;

    // Mutation
    void swapMutation(std::vector<int>& chromosome, double mutationRate) const;

    // Replacement
    void solutionReplacer(std::vector<std::vector<int>>& originalPopulation,
                          const std::vector<std::vector<int>>& solutionMatrix) const;
};

#endif // TSP_GENETIC_ALGORITHM_HPP