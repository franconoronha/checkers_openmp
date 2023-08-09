c:
	g++ main.cpp checkers.cpp -fopenmp -o checkers
warnings:
	g++ main.cpp checkers.cpp -fopenmp -o checkers -Wall -pedantic