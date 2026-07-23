# Compiler
CXX = g++

# Executable name
CXXFLAGS = -std=c++11 -Wall -Wextra

.PHONY: clean

brute_force_main: brute_force_main.o tsp_brute_force.o
	$(CXX) $(CXXFLAGS) brute_force_main.o tsp_brute_force.o -o brute_force_main

brute_force_main.o: brute_force_main.cpp tsp_brute_force.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tsp_brute_force.o: tsp_brute_force.cpp tsp_brute_force.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o brute_force_main

main-exist:
	@if [ $(FILE_EXISTS) -eq 0 ]; then \
		echo "File main does not exist. Compile it first!"; \
		exit 1; \
	fi

test-run: main-exist
	@for param in 5, 6, 7; do \
		./main cities.csv $$param; \
	done
	@echo "PASSED"

FILE_EXISTS := $(shell if [ -e "main" ]; then echo 1; else echo 0; fi)
STRING_TO_MATCH = "112.4"
TEST_RUNS = 3
test-result: main-exist
	@correct=0; \
	for i in $$(seq 1 $(TEST_RUNS)); do \
		output=$$(./main cities.csv 7); \
		echo "$$output"; \
		if echo "$$output" | grep -q "$(STRING_TO_MATCH)"; then \
			echo "Test $$i: PASSED"; \
			correct=1; \
			break; \
		else \
			echo "Test $$i: FAILED"; \
		fi; \
	done; \
