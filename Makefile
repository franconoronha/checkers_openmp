c:
	mpic++ main.cpp checkers.cpp -o checkers  
run:
	mpirun -np 2 ./checkers