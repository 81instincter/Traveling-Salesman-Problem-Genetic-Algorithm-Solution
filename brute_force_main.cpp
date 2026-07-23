/**
 * Brute-force (exhaustive) driver for the Traveling Salesman Problem.
 *
 * Usage: ./brute_force_main <cities.csv> <number_of_cities>
 *
 * Warning: complexity is O((n-1)!). Practical only for n ≤ 11–12.
 */

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "tsp_brute_force.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <number_of_cities>\n";
        return 1;
    }

    const std::string inputFileName = argv[1];
    const int numberOfCities = std::stoi(argv[2]);

    if (numberOfCities < 2 || numberOfCities > 12) {
        std::cerr << "For brute force, number of cities should be between 2 and 12 "
                     "(factorial growth).\n";
        return 1;
    }

    std::ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file: " << inputFileName << "\n";
        return 1;
    }

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

    TSPBruteForce solver(cityGraph, numberOfCities);

    const auto start = std::chrono::high_resolution_clock::now();
    const double bestLength = solver.computeShortestTour();
    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "=== Brute-Force Result ===\n";
    std::cout << "Cities examined      : " << numberOfCities << "\n";
    std::cout << "Best tour length     : " << bestLength << "\n";
    std::cout << "Runtime              : " << duration << " ms\n";
    std::cout << "Best tour            : ";
    for (int city : solver.getBestTour()) {
        std::cout << city << " ";
    }
    if (!solver.getBestTour().empty()) {
        std::cout << solver.getBestTour().front() << " (return)";
    }
    std::cout << "\n";

    // Optional timing log
    std::fstream fout("bfTimes.csv", std::ios::out | std::ios::app);
    if (fout) {
        fout << duration << "\n";
        fout.close();
    }

    return 0;
}