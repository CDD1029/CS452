#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <bits/stdc++.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah
int calcMid(int first, int last);
void mergesort(int * a, int first, int last, int * sorted);
void smerge(int * a, int first1, int last1, int first2, int last2, int * sorted);
int rank(int * a, int first, int last, int valtofind);
void pmerge(int * a, int first, int last, int mid, int * sorted);

int calcMid(int first, int last){
	return first + (last - first) / 2;
}

void mergesort(int * a, int first, int last, int * sorted){ // works up to last, not including last index
	if (last > first + 1){
		cout << first << ":" << last << endl;
		int m = calcMid(first,last);
		mergesort(&a[first], 0, m, &sorted[first]); 
		mergesort(&a[m], 0, last-m, &sorted[m]);
		pmerge(a, first, last, m, sorted);
	}
}

void smerge(int * a, int first1, int last1, int first2, int last2, int * sorted){
	int * left = &a[first1];
	int leftsize = last1 - first1;
	int * right = &a[first2];
	int rightsize = last2 - first2;
	int leftindex = 0;
	int rightindex = 0;
	int sortedindex = 0;
	while (rightindex<rightsize&&leftindex<leftsize){
		if (right[rightindex]<=left[leftindex])
			sorted[sortedindex++]=right[rightindex++];
		sorted[sortedindex++]=left[leftindex++];
	}
	while (leftindex<leftsize)
		sorted[sortedindex++]=left[leftindex++];
	while (right[rightindex])
		sorted[sortedindex++]=right[rightindex++];
}

int rank(int * a, int first, int last, int valtofind){
	if(first==last)
		return first;
	if (first>last)
		return rank(a, last, first, valtofind);
	if (first + 1 == last){
		if (a[first]>=valtofind)
			return first;
		return last;
	}
	int m = first + (last-first)/2);
	if (a[m]<valtofind)
		return rank(a, first, m, valtofind);
	if (a[m]>valtofind)
		return rank(a, m+1, last, valtofind);
	return m;
}

void pmerge(int * a, int first, int last, int mid, int * sorted){
	int n = last - first;
	int numberofrankstocalculate = (int) n/log(n);
}

int main (int argc, char * argv[]) {

	int my_rank;			// my CPU number for this process
	int p;					// number of CPUs that we have
	int source;				// rank of the sender
	int dest;				// rank of destination
	int tag = 0;			// message number
	char message[100];		// message itself
	const int root = 0;
	MPI_Status status;		// return status for receive
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	int arraysize;
	
	if(my_rank == root) {
		arraysize = 5;
	}
	MPI_Bcast(&arraysize, 1, MPI_INT, root, MPI_COMM_WORLD);
	int * array = new int[arraysize];
	int * sorted = new int[arraysize]; // destination array
	if (my_rank==root)
		for (int i=0; i<arraysize; i++)
			array[i]= arraysize - i;
	MPI_Bcast(array, arraysize, MPI_INT, root, MPI_COMM_WORLD);
	// Shut down MPI
	MPI_Finalize();

	return 0;
}
