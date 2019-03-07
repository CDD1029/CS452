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

void mergesort(int * a, int first, int last);
void smerge(int * a, int first1, int last1, int first2, int last2);
int rank(int * a, int first, int last, int valtofind);
void pmerge(int * a, int first, int last, int mid);

void mergesort(int * a, int first, int last){
	if (last > first + 1){
		cout << first << ":" << last << endl;
		int m = first + (last - first) / 2;
		mergesort(a, first, m);
		mergesort(a, m, last);
		pmerge(a, first, last, mid);
	}
	return;
}

void smerge(int * a, int first1, int last1, int first2, int last2){
	int * left = &a[first1];
	int leftsize = last1 - first1;
	int * right = &a[first2];
	int rightsize = last2 - first2;
	int mergedsize = leftsize + rightsize;
	int * merged = new int[mergedsize];
	int leftindex = 0;
	int rightindex = 0;
	int mergedindex = 0;
	while (mergedindex < mergedsize){
		while (rightindex < rightsize){
			while (leftindex<leftsize){
				if (left[leftindex] <= right[rightindex])
					merged[mergedindex++]=left[leftindex++];
				else
					merged[mergedindex++]=right[rightindex++];
			}
			merged[mergedindex++]=right[rightindex++];
		}
		merged[mergedindex++]=left[leftindex++];
	}
	
}

int rank(int * a, int first, int last, int valtofind){
	int m = first + (last - first) / 2;
	if (last > first + 1){
		if (a[m] > valtofind)
			return rank(a, m, last, valtofind);
		if (a[m] < valtofind)
			return rank(a, first, m, valtofind);
	}
	return m;
}

void pmerge(int * a, int first, int last, int mid){
	return;
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
	
	if (my_rank==0){
		srand(1231);
		int arraysize=0;
		cout << "Enter size of array:" << endl;
		cin >> arraysize;
		int * array = new int[arraysize];
		
		for (int i=0; i<arraysize; i++){
			array[i]=rand()%10;
		}
		cout<< "Unsorted:"<<endl;
		for (int i=0; i < arraysize; i++){
			cout<< array[i] << endl;
		}
		
		mergesort(array, 0, arraysize);
		cout<< "Sorted:"<<endl;
		for (int i=0; i < arraysize; i++){
			cout<< array[i] << endl;
		}
	}
	
	// Shut down MPI
	MPI_Finalize();

	return 0;
}
