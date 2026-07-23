/**
 * Genetic Algorithm driver for the Traveling Salesman Problem.
 *
 * Usage: ./main <cities.csv> <number_of_cities>
 *
 * Example:
 *   ./main cities.csv 8
 */

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "tsp_genetic_algorithm.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <number_of_cities>\n";
        return 1;
    }

    // Deterministic seed for reproducible demos (change for true randomness)
    std::srand(42);

    const std::string inputFileName = argv[1];
    const int numberOfCities = std::stoi(argv[2]);

    if (numberOfCities < 2 || numberOfCities > 20) {
        std::cerr << "Number of cities must be between 2 and 20.\n";
        return 1;
    }

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file: " << inputFileName << "\n";
        return 1;
    }

    // Read full 20×20 distance matrix (only the top-left n×n is used)
    std::vector<std::vector<double>> cityGraph(20, std::vector<double>(20, 0.0));
    std::string line;
    int row = 0;
    while (std::getline(inputFile, line) && row < 20) {
        std::stringstream ss(line);
        std::string value;
        int col = 0;
        while (std::getline(ss, value, ',') && col < 20) {
            cityGraph[row][col] = std::stod(value);
            ++col;
        }
        ++row;
    }
    inputFile.close();

    // Configure GA
    TSPGeneticAlgorithm ga(numberOfCities, 0.8, 0.15);
    ga.setPopulationSize(8);          // results in 80 individuals
    ga.setNumberOfGenerations(20);    // not used in the shrinking-loop design
    ga.setDistanceMatrix(cityGraph);

    auto population = ga.initialPopulationGenerator(
        ga.getNumCities(), ga.getPopulationSize());

    std::cout << "Initial population size: " << population.size() << "\n";

    const auto start = std::chrono::high_resolution_clock::now();

    // Iteratively select & breed until only two elite solutions remain
    int generation = 0;
    while (population.size() > 2) {
        auto parents = ga.tournamentSelection(population, 3, 4);
        auto offspring = ga.pmxCrossoverBreedingFitness(population, ga.getCrossoverRate());

        for (auto& child : offspring) {
            ga.swapMutation(child, ga.getMutationRate());
        }

        ga.solutionReplacer(population, offspring);
        ++generation;

        if (generation % 5 == 0) {
            std::cout << "Generation " << generation
                      << " – population size: " << population.size() << "\n";
        }
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Evaluate the final elite solutions
    double bestFitness = -1.0;
    std::vector<int> bestTour;
    for (const auto& tour : population) {
        double f = ga.fitness(tour, cityGraph);
        if (f > bestFitness) {
            bestFitness = f;
            bestTour = tour;
        }
    }

    // Convert fitness back to approximate tour length for readability
    const double approxLength = (bestFitness > 0.0) ? (1000.0 / bestFitness) : 0.0;

    std::cout << "\n=== Genetic Algorithm Result ===\n";
    std::cout << "Generations executed : " << generation << "\n";
    std::cout << "Runtime              : " << duration << " ms\n";
    std::cout << "Best tour length     : " << approxLength << "\n";
    std::cout << "Best tour            : ";
    for (int city : bestTour) {
        std::cout << city << " ";
    }
    std::cout << bestTour.front() << " (return)\n";  // show cycle

    // Optional timing log (append)
    std::fstream fout("gaTimes.csv", std::ios::out | std::ios::app);
    if (fout) {
        fout << duration << "\n";
        fout.close();
    }

    return 0;
}