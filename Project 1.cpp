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
    	// cout << my_rank << endl;
	
	// Two Rings
	
	if (my_rank == 0 || my_rank == 1) {
		// the initial messages are sent
		if (my_rank == 0) 
			sprintf(message, "Evens ");
		else
			sprintf(message, "Odds ");
		MPI_Send(message, strlen(message)+1, MPI_CHAR, my_rank + 2, tag, MPI_COMM_WORLD);
	}
	// everyone has got to recieve the message
	MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
	cout << message << my_rank << endl;
	if (my_rank >1) {
		// if you're not the originator then you have to send it to the next processor
		if (my_rank+2 < p)
			MPI_Send(message, strlen(message)+1, MPI_CHAR, my_rank + 2, tag, MPI_COMM_WORLD);
		else 
			MPI_Send(message, strlen(message)+1, MPI_CHAR, my_rank % 2, tag, MPI_COMM_WORLD);
	} else
	{
		cout << message << my_rank << endl;
	}
	
	
	
	// Whack-an-Orc
	
	// In Your EYE

	// Shut down MPI
	MPI_Finalize();

	return 0;
}
