#include <iostream>
#include <stdio.h>
#include <string.h>
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
/*
	int sum = 0;
	for (int x = 0; x < 300000; x++)
		sum += x;
	cout << sum << endl;
*/
	// Sebastian Wernicke
		
	// KNOW WHAT THE PROBLEM IS
	int n = 300000;
	long sum = 0;
	
	// BREAK THE PROBLEM UP INTO PIECES
	int local_start = n/p*my_rank;
	
	// EACH PROCESS DOES ITS OWN LOCAL WORK
	long local_sum = 0;
	for (int x = local_start; x < local_start + n/p; x++)
		local_sum += x;
	
	// GLUE THOSE LOCAL RESULTS BACK TOGETHER
	// The processor in charge is called the overseer.
	// By convention, the overseer is usually process 0.
	
	// If I am not the overeer, send my answer to the overseer
	if (my_rank != 0)
		MPI_Send(&local_sum, 1, MPI_LONG, 0, tag, MPI_COMM_WORLD);
	else {
		sum = local_sum;
		long temp;
		for (int x = 1; x < p; x++) {
			MPI_Recv(&temp, 1, MPI_LONG, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			sum += temp;
		} // end for
		cout << sum << endl;
	} // end else

	// Shut down MPI
	MPI_Finalize();

	return 0;
}
