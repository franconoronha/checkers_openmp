c:
	g++ main.cpp checkers.cpp -fopenmp -lpthread -o checkers
warnings:
	g++ main.cpp checkers.cpp -fopenmp -lpthread -o checkers -Wall -pedantic