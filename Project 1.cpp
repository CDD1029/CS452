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
	
	char * evens = message;
	sprintf(message, "Evens");
	char * odds = message;
	sprintf(message, "Odds");
	
	if (my_rank == 0 || my_rank == 1) {
		// send the messages and then recieve them
	}
	else if (my_rank+2 <= p){
		// recieve the message then send to my_rank+2
	}
	else {
		// recieve the message then send to 1 or 0
	}
	
	// Whack-an-Orc
	
	// In Your EYE

	// Shut down MPI
	MPI_Finalize();

	return 0;
}
