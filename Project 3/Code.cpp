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

void mergesort(int * a, int first, int last, int * sorted);
void smerge(int * a, int first1, int last1, int first2, int last2, int * sorted);
int rank(int * a, int first, int last, int valtofind);
void pmerge(int * a, int first, int last, int mid, int * sorted);

void mergesort(int * a, int first, int last, int * sorted){
	if (last > first + 1){
		cout << first << ":" << last << endl;
		int m = first + (last - first) / 2;
		mergesort(&a[first], 0, m, &sorted[first]);
		mergesort(&a[m], 0, last-m, &sorted[m]);
		int index = first;
		while (index < last)
			a[index]=sorted[index];
		pmerge(a, first, last, m, sorted);
		index = first;
		while (index < last)
			a[index]=sorted[index];
	}
}

void smerge(int * a, int first1, int last1, int first2, int last2, int * sorted){
	if (first1>last1){
		smerge(a,last1,first1,first2,last2,sorted);
		return;
	}
	if (first2>last2){
		smerge(a,first1,last1,last2,first2,sorted);
		return;
	}
	int * left = &a[first1];
	int * right = &a[first2];
	int leftsize = last1-first1;
	int rightsize = last2-first2;
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
	while (rightindex<rightsize)
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
	int * left = &a[first];
	int * right = &a[mid];
	int leftsize = mid - first;
	int rightsize = last - mid;
	int leftchunk = log(leftsize);
	int rightchunk = log(rightsize);
	int lranksize = leftsize/leftchunk
	int * leftranks = new int[lranksize];
	int rranksize = rightsize/rightchunk
	int * rightranks = new int[rranksize];
	int leftindex = 0;
	int rightindex = 0;
	while(leftindex<lranksize){
		leftranks[leftindex]=rank(right, 0, rightsize, left[leftindex*leftchunk])+(leftindex*leftchunk);
		leftindex++;
	}
	while(rightindex<rranksize){
		rightranks[rightindex]=rank(left, 0, leftsize, right[rightindex*rightchunk])+(rightindex*rightchunk);
		rightindex++;
	}
	leftindex=0;
	rightindex=0;
	while(leftindex<lranksize&&rightindex<rranksize){
		int first1=leftindex*leftchunk;
		int first2=rightindex*rightchunk;
		int last1=rightranks[rightindex]-first2;
		int last2=leftranks[leftindex]-first1;
		if (leftranks[leftindex]<rightranks[rightindex]){
			smerge(a, first1, last1, first2+mid, last2+mid, &sorted[leftranks[leftindex]]);
			leftindex++;
		}
		else{
			smerge(a, first1, last1, first2+mid, last2+mid, &sorted[rightranks[rightindex]]);
			rightindex++;
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
	
	if (my_rank==0){
		srand(1231);
		int arraysize=0;
		cout << "Enter size of array:" << endl;
		cin >> arraysize;
		int * array = new int[arraysize];
		int * sorted = new int[arraysize];
		for (int i=0; i<arraysize; i++){
			array[i]= arraysize - i;
			// rand()%100
		}
		cout<< "Unsorted:"<<endl;
		for (int i=0; i < arraysize; i++){
			cout<< array[i] << endl;
		}
		
		mergesort(array, 0, arraysize, sorted);
		cout<< "Sorted:"<<endl;
		for (int i=0; i < arraysize; i++){
			cout<< array[i] << endl;
		}
	}
	
	// Shut down MPI
	MPI_Finalize();

	return 0;
}
