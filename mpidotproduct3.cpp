#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int source;				// rank of the sender
	int dest;				// rank of destination
	int tag = 0;			// message number
	char message[100];		// message itself
	MPI_Status status;		// return status for receive
	
	// Start MPI
	MPI_Init(&argc, &argv);
	
	// Find out my rank!
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	// Find out the number of processes!
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	// THE REAL PROGRAM IS HERE
	// srand(time(0));
/* 	
	int a[10], b[10];
	for (int x = 0; x < 10; x++) {
		a[x] = rand() % 10;
		b[x] = rand() % 10;
	}
	
	int dotproduct = 0;
	for (int x = 0; x < 10; x++)
		dotproduct += a[x]*b[x];
	 
 */
	// Know the problem.
	// Hint: It is still you.
	
	srand(1231);
	int n = 300000;
	int * a = new int[n];
	int * b = new int[n];
	long int dotproduct = 0;
	
	if (my_rank == 0)
		for (int x = 0; x < n; x++) {
			a[x] = rand() % 8;
			b[x] = rand() % 8;
		}
	
	// Divide the problem up
	int localn = n/p;
	int * locala = new int[localn];
	int * localb = new int[localn];	

	// After executing these lines of code, locala and localb will have the "right info"
	// Each will have the block of array a (or b) that corresponds to their local rank
	// Each block is of size localn
	MPI_Scatter(&a[0], localn, MPI_INT, locala, localn, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(&b[0], localn, MPI_INT, localb, localn, MPI_INT, 0, MPI_COMM_WORLD);
	
	// Do the local work
	long int localdot = 0;
	for (int x = 0; x < localn; x++)
		localdot += locala[x] * localb[x];
	
	// Collect the results back together.
	for (int x = 0; x < p; x++)
		MPI_Reduce(&localdot, &dotproduct, 1, MPI_LONG, MPI_SUM, x, MPI_COMM_WORLD);
	
	if (my_rank == 0)
		cout << dotproduct << endl;
	
	// Shut down MPI
	MPI_Finalize();
	
	delete [] a;
	delete [] b;
	delete [] locala;
	delete [] localb;

	return 0;
}
