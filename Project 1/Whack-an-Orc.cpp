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
    srand(1251);
	int n = 300000;
	int * orcs = new int[n];
	long int min=0;
	long int max=0;
	long int avg=0;
    // making the array
	if (my_rank==0)
		for (int x=0; x<n; x++)
			orcs[x]=rand();
	int localn=n/p;
	int * localorcs=new int[localn];

	MPI_Scatter(&orcs[0], localn, MPI_INT, localorcs, localn, MPI_INT, 0, MPI_COMM_WORLD);
	// min
	long int localmin=localorcs[0];
	for (int x=1; x<localn; x++)
		if (localmin>localorcs[x])
			localmin=localorcs[x];
	MPI_Allreduce(&localmin, &min, 1, MPI_LONG, MPI_MIN, MPI_COMM_WORLD);

	if (my_rank==0)
		cout << min << endl;
    //max
	long int localmax=localorcs[0];
	for (int x=1;x<localn;x++)
		if (localmax<localorcs[x])
			localmax=localorcs[x];
	MPI_Allreduce(&localmax, &max, 1, MPI_LONG, MPI_MAX, MPI_COMM_WORLD);
	if (my_rank==0)
		cout<< max << endl;
    // avg
	long int localavg=0;
	for (int x=0; x<localn; x++)
		localavg+=localorcs[x]/localn;
	MPI_Allreduce(&localavg, &avg, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
	if (my_rank==0)
		cout<< avg/p << endl;
	
    delete [] orcs;
	delete [] localorcs;

	// Shut down MPI
	MPI_Finalize();

	return 0;
}
