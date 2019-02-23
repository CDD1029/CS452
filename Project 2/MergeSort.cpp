#include <iostream>
#include <stdio.h>
#include <string.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah

void mergesort(int * a, int first, int last);
void merge(int * a, int * b, int first, int last, int middle);

void mergesort(int * a, int first, int last){
	if (last > first){
		int m = first+(last-first)/2;
		mergesort(a, first, m);
		mergesort(a, m, last);
		int * b = a[m];
		merge(a, b, first, last, m);
	}
	return;
}

void merge(int * a, int * b, int first, int last, int middle){
	int aplace = 0;
	int bplace = 0;
	while (aplace<middle && bplace<last-middle){
		if (a[aplace] <= b[bplace])
			aplace++;
		else {
			int value = b[bplace];
			int index = bplace + middle;
			
			while (index!=aplace){
				a[index] = a[index - 1]; 
                		index--;
			}
			a[aplace] = value;
			
			aplace++;
			bplace++;
			middle++;
		}
	}
}

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
    cout << my_rank << endl;

	// Shut down MPI
	MPI_Finalize();

	return 0;
}
