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
    	// cout << my_rank << endl;
	
	// Two Rings
/*
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
	}
*/	
	
	
	// Whack-an-Orc
/*	
	srand(1251);
	int n = 300000;
	int * orcs = new int[n];
	long int min=0;
	long int max=0;
	long int avg=0;

	if (my_rank==0)
		for (int x=0; x<n; x++)
			orcs[x]=rand();
	int localn=n/p;
	int * localorcs=new int[localn];

	MPI_Scatter(&orcs[0], localn, MPI_INT, localorcs, localn, MPI_INT, 0, MPI_COMM_WORLD);
	
	long int localmin=localorcs[0];
	for (int x=1; x<localn; x++)
		if (localmin>localorcs[x])
			localmin=localorcs[x];
	MPI_Allreduce(&localmin, &min, 1, MPI_LONG, MPI_MIN, MPI_COMM_WORLD);

	if (my_rank==0)
		cout << min << endl;

	long int localmax=localorcs[0];
	for (int x=1;x<localn;x++)
		if (localmax<localorcs[x])
			localmax=localorcs[x];
	MPI_Allreduce(&localmax, &max, 1, MPI_LONG, MPI_MAX, MPI_COMM_WORLD);
	if (my_rank==0)
		cout<< max << endl;

	long int localavg=0;
	for (int x=0; x<localn; x++)
		localavg+=localorcs[x]/localn;
	MPI_Allreduce(&localavg, &avg, 1, MPI_LONG, MPI_SUM, MPI_COMM_WORLD);
	if (my_rank==0)
		cout<< avg/p << endl;
	delete [] orcs;
	delete [] localorcs;
*/
	// In Your EYE
	
	int n = 106;
	char * letters=new char[n];
	int * counts=new int[26];
	int i=0;
	int localn=n/p;
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
	int * localcounts=new int[26];
	for (int x=0;x<26;x++)
		localcounts[x]=0;
	for (int x=0; x<localn; x++){
		int place=(int)localletters[x];
		place -= 97;
		if (place >=0 && place < 26)
			localcounts[place]++;
	}
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
