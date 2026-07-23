# TSP Genetic Algorithm & Brute-Force Solver

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![Build](https://img.shields.io/badge/build-passing-brightgreen.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

Educational C++ implementation of two classic approaches to the **Traveling Salesman Problem (TSP)**:

1. **Genetic Algorithm** – population-based heuristic using tournament selection, Partially-Mapped Crossover (PMX), and swap mutation.
2. **Brute-Force (Exhaustive Search)** – examines all unique tours (optimised to `(n-1)!`) and returns the globally optimal solution.

Originally developed as a course project exploring combinatorial optimisation and evolutionary algorithms. Polished for portfolio / recruiter review.

---

## Features

| Component | Highlights |
|-----------|------------|
| **Genetic Algorithm** | Tournament selection · PMX crossover · Swap mutation · Fitness = inverse tour length · Population reduction to elite set |
| **Brute Force** | Full enumeration of distinct cycles · Starting-city fixed to eliminate rotational symmetry · Exact optimum |
| **Shared data** | 20×20 distance matrix supplied via CSV (symmetric) |
| **Timing** | High-resolution clock measurements written to `gaTimes.csv` / `bfTimes.csv` |
| **Modern C++** | C++17 · `const`-correct APIs · No `using namespace` in headers · Clean Rule-of-Zero style classes |

---

## Project Structure
```
TSP-Genetic-Algorithm/
├── main.cpp                    # Genetic Algorithm driver
├── brute_force_main.cpp        # Exhaustive-search driver
├── tsp_genetic_algorithm.hpp   # GA class declaration
├── tsp_genetic_algorithm.cpp   # GA implementation (selection, PMX, mutation …)
├── tsp_brute_force.hpp         # Brute-force class declaration
├── tsp_brute_force.cpp         # Brute-force implementation
├── cities.csv                  # Sample 20×20 distance matrix
├── Makefile                    # One-command build for both targets
├── .gitignore
├── LICENSE
└── README.md
```
---

## Building & Running

### Prerequisites
- C++17 compiler (`g++` ≥ 7 or `clang++`)
- `make` (recommended)

### Quick start

```bash
# Clone / download
cd TSP-Genetic-Algorithm

# Build both programs
make

# Run Genetic Algorithm on 8 cities
make run-ga
# or
./main cities.csv 8

# Run brute-force on the same instance (exact optimum)
make run-bf
# or
./brute_force_main cities.csv 8
```

## Sample Output
### Genetic Algorithm (8 Cities)
```
Initial population size: 80
Generation 5 – population size: 40
...
=== Genetic Algorithm Result ===
Generations executed : 12
Runtime              : 4 ms
Best tour length     : 142.3
Best tour            : 0 3 7 2 5 1 6 4 0 (return)
```

### Brute Force (8 Cities)
```
=== Brute-Force Result ===
Cities examined      : 8
Best tour length     : 138.7
Runtime              : 18 ms
Best tour            : 0 4 1 5 2 7 3 6 0 (return)
```

## Technical Highlights
- **Classic evolutionary operators** implemented from first principles (no external GA library).
- **PMX crossover** correctly maintains permutation validity – a non-trivial detail often glossed over in tutorials.
- **Const-correct** public interface and careful use of references to avoid unnecessary copies.
- **Complexity awareness:** brute-force limited to n ≤ 12 with clear documentation of the factorial explosion.
- **Clean separation of concerns:** distance matrix loading, algorithm classes, and thin driver programs.
- **Reproducible experiments** via fixed srand seed (easily changed for stochastic runs).
- Modern C++17 style while remaining approachable for beginners still learning the language.

## Known Limitations & Future Work

- GA is a simple elitist / population-shrinking design rather than a classic generational or steady-state GA.
- Distance matrix is assumed symmetric and complete; no support for asymmetric or sparse graphs.
- No parallel evaluation of fitness or multi-threaded population updates.
- Fitness scaling is a simple reciprocal; more sophisticated ranking or linear scaling could be added.
- Visualisation of tours (e.g. with a plotting library) would make results more intuitive.

# Author
**Elijah Kenning**

# License
This project is released under the MIT License.
Feel free to use it as a reference or starting point for your own combinatorial-optimisation experiments.
