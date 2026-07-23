#include "tsp_brute_force.hpp"

#include <algorithm>
#include <limits>
#include <vector>

TSPBruteForce::TSPBruteForce(const std::vector<std::vector<double>>& cityGraph,
                             int numberOfCities)
    : cityGraph_(cityGraph), numberOfCities_(numberOfCities) {}

double TSPBruteForce::computeShortestTour() {
    std::vector<int> tour(numberOfCities_);
    for (int i = 0; i < numberOfCities_; ++i) {
        tour[i] = i;
    }

    double shortestDistance = std::numeric_limits<double>::max();
    bestTour_.clear();

    // Fix city 0 as the starting point to avoid counting rotations as distinct
    // (optional optimisation; still examines (n-1)! tours)
    do {
        double current = computeTourDistance(tour);
        if (current < shortestDistance) {
            shortestDistance = current;
            bestTour_ = tour;
        }
    } while (std::next_permutation(tour.begin() + 1, tour.end()));

    return shortestDistance;
}

double TSPBruteForce::computeTourDistance(const std::vector<int>& tour) const {
    double distance = 0.0;
    for (int i = 0; i < numberOfCities_ - 1; ++i) {
        distance += cityGraph_[tour[i]][tour[i + 1]];
    }
    // Return to start
    distance += cityGraph_[tour[numberOfCities_ - 1]][tour[0]];
    return distance;
}

const std::vector<int>& TSPBruteForce::getBestTour() const {
    return bestTour_;
}