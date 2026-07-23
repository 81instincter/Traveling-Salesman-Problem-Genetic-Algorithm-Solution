### Makefile

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -O2
LDFLAGS  :=

# Targets
.PHONY: all clean run-ga run-bf help

all: main brute_force_main

main: main.o tsp_genetic_algorithm.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

brute_force_main: brute_force_main.o tsp_brute_force.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

main.o: main.cpp tsp_genetic_algorithm.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o $@

tsp_genetic_algorithm.o: tsp_genetic_algorithm.cpp tsp_genetic_algorithm.hpp
	$(CXX) $(CXXFLAGS) -c tsp_genetic_algorithm.cpp -o $@

brute_force_main.o: brute_force_main.cpp tsp_brute_force.hpp
	$(CXX) $(CXXFLAGS) -c brute_force_main.cpp -o $@

tsp_brute_force.o: tsp_brute_force.cpp tsp_brute_force.hpp
	$(CXX) $(CXXFLAGS) -c tsp_brute_force.cpp -o $@

# Convenience targets
run-ga: main
	./main cities.csv 8

run-bf: brute_force_main
	./brute_force_main cities.csv 8

clean:
	rm -f *.o main brute_force_main gaTimes.csv bfTimes.csv

help:
	@echo "TSP Genetic Algorithm + Brute Force"
	@echo ""
	@echo "  make              Build both solvers"
	@echo "  make run-ga       Build & run GA on 8 cities"
	@echo "  make run-bf       Build & run brute force on 8 cities"
	@echo "  make clean        Remove build artifacts"
	@echo ""
	@echo "Manual usage:"
	@echo "  ./main cities.csv <n>                 (n = 2..20)"
	@echo "  ./brute_force_main cities.csv <n>     (n = 2..12 recommended)"