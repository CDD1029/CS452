#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
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
	
	int n = 106;
	char * letters=new char[n];
	int * counts=new int[26];
	int i=0;
	int localn=n/p;
	// making the array from the file
	if (my_rank==0){
		ifstream file ("text.txt");
		if (file.is_open()){
			while (!file.eof()){
				file>>letters[i];
				i++;
			}
			file.close();
		}
	}
	char * localletters= new char[localn];
	MPI_Scatter(&letters[0], localn, MPI_CHAR, localletters, localn, MPI_CHAR, 0, MPI_COMM_WORLD);
	// making localcounts and then adding up the counts from each part
	int * localcounts=new int[26];
	for (int x=0;x<26;x++)
		localcounts[x]=0;
	for (int x=0; x<localn; x++){
		int place=(int)localletters[x];
		place -= 97;
		if (place >=0 && place < 26)
			localcounts[place]++;
	}
	// sending it back then tidying up
	for (int x=0;x<26;x++)
		MPI_Allreduce(&localcounts[x], &counts[x], 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	if (my_rank==0)
		for (int x=0; x<26; x++)
			cout << (char)(x+97) << ":" << counts[x] << endl;
	
	delete [] letters;
	delete [] counts;
	delete [] localletters;
	delete [] localcounts;

	// Shut down MPI
	MPI_Finalize();

	return 0;
}
