#include <iostream>
#include <stdio.h>
#include <string.h>
#include "mpi.h" // message passing interface
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah
void mergesort(int * a, int first, int last, int * sorted, int my_rank);
void smerge(int * a, int first1, int last1, int first2, int last2, int * sorted);
int rank(int * a, int first, int last, int valtofind);
void pmerge(int * a, int first, int last, int mid, int * sorted);
int calcMid(int first, int last){
	return first + (last - first) / 2;
}
int calcFShapey(int logn, int size,int iteration) {
	int toSend = logn * iteration + 1; 
	if(toSend >= size) return -1; //error
	return toSend;
}
int calcLShapey(int logn, int size,int iteration) {
	int toSend = logn * (iteration + 1) -1 ; 
	if(toSend >= size) return size -1;
	return toSend;
}
void SHAPESYAY(int * shapeyshape,int * shapeyshape2, int numberOfShapeyShapes, int logn, int size, int * srank) {
	int i = 0; 
	int iteration = 0;
	int first = calcFShapey(logn, size, iteration);
	int last = calcLShapey(logn, size, iteration);
	iteration++;
	int shapeyIndex = 0;
	int prevSrank = -1;
	while(shapeyIndex < numberOfShapeyShapes){
		int toCheck = srank[i];
		if(toCheck <= last) {
			if(toCheck < first||srank[i] == srank[i-1]) {
				shapeyshape[shapeyIndex] = 0; // just a null for later
				shapeyshape2[shapeyIndex] = -1;
				shapeyIndex++;
			}
			else if(toCheck == first){

				shapeyshape[shapeyIndex] = 1; 
				shapeyshape2[shapeyIndex] = first;
				first = first + 1;
				shapeyIndex++;
			}
			else if(toCheck > first){
				shapeyshape[shapeyIndex] = toCheck - first; // the +1 that normally is needed is negated 
				shapeyshape2[shapeyIndex] = toCheck - 1;
				shapeyIndex++;
				first = toCheck;
			}
			i++;
			if(i >= logn) {
				cout << "yo" << endl;
				shapeyshape[shapeyIndex] = last - first + 1; 
				shapeyshape2[shapeyIndex] = last;
				shapeyIndex++;
				while(shapeyIndex < numberOfShapeyShapes){
				shapeyshape[shapeyIndex] = calcFShapey(logn, size, iteration)-calcLShapey(logn, size, iteration)+1;
				shapeyshape2[shapeyIndex] = calcLShapey(logn, size, iteration);
				shapeyIndex++;				
				iteration++;
				}
			}
		}
		else{
			shapeyshape[shapeyIndex] = last - first+1; 
			shapeyshape2[shapeyIndex] = last;
			shapeyIndex++;
			first = calcFShapey(logn, size, iteration);
			last = calcLShapey(logn, size, iteration);
			iteration++;
		}
	}
}
void mergesort(int * a, int first, int last, int * sorted, int my_rank){ // works up to last, not including last index
	if (last > first + 1){
		cout << first << ":" << last << endl;
		int m = calcMid(first,last);
		mergesort(&a[first], 0, m, &sorted[first],my_rank); 
		mergesort(&a[m], 0, last-m, &sorted[m],my_rank);
		pmerge(a, first, last, m, sorted);
	}
}
int rank(int * a, int first, int last, int valtofind){
	for(int i = first; i < last; i++) cout << a[i] << " a" << valtofind << endl;
	cout << first << " f" << last << "l" << endl;
	if(first==last)
		return first;
	if (first + 1 == last){
		if (a[first]>=valtofind)
			return first;
		return last;
	}
	int m = calcMid(first,last);
	if (valtofind<a[m])
		return rank(a, first, m, valtofind);
	if (valtofind>a[m])
		return rank(a, m, last, valtofind);
	return m;
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
int log(int a) {
	return 4;// todo write a while loop to actually do this
}
void pmerge(int * a, int first, int last, int mid, int * sorted){
	int * left = &a[first];
	int * right = &a[mid];
	int leftsize = mid - first;
	int rightsize = last - mid;
	int leftchunk = log(leftsize);
	int rightchunk = log(rightsize);
	int lranksize = leftsize/leftchunk;
	int * leftranks = new int[lranksize];
	int rranksize = rightsize/rightchunk;
	int * rightranks = new int[rranksize];
	int leftindex = 0;
	int rightindex = 0;
	while(leftindex<lranksize){
		cout << left[leftindex*leftchunk] << endl;
		leftranks[leftindex]=rank(right, 0, rightsize, left[leftindex*leftchunk]);
				cout << leftranks[leftindex] << endl;

		leftindex++;
	}
	while(rightindex<rranksize){
		rightranks[rightindex]=rank(left, 0, leftsize, right[rightindex*rightchunk]);
		rightindex++;
	}
	// There is a pic on Brianna's phone of what to do here but we all good bois
		cout << "ranksz" << endl;
	int * shapeyshapesf = new int[8];
	int * shapeyshapesl = new int[8]; // idk how to do multidimensional

	SHAPESYAY(shapeyshapesf,shapeyshapesl,8,4,16,leftranks);
	for(int i = 0; i < 8; i++) 
		cout << shapeyshapesf[i] << "|| " << shapeyshapesl[i] << endl;
		cout << "ranksz" << endl;

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
		arraysize = 32;
	}
	MPI_Bcast(&arraysize, 1, MPI_INT, root, MPI_COMM_WORLD);
	int * array = new int[arraysize];
	int * sorted = new int[arraysize]; // destination array
	int x = 0; 
	array[x++] = 4;
	array[x++] = 6;
	array[x++] = 8;
	array[x++] = 9;
	array[x++] = 16;
	array[x++] = 17;
	array[x++] = 18;
	array[x++] = 19;
	array[x++] = 20;
	array[x++] = 21;
	array[x++] = 23;
	array[x++] = 25;
	array[x++] = 27;
	array[x++] = 29;
	array[x++] = 31;
	array[x++] = 32;
	array[x++] = 1;
	array[x++] = 2;
	array[x++] = 3;
	array[x++] = 5;
	array[x++] = 7;
	array[x++] = 10;
	array[x++] = 11;
	array[x++] = 12;
	array[x++] = 13;
	array[x++] = 14;
	array[x++] = 15;
	array[x++] = 22;
	array[x++] = 24;
	array[x++] = 26;
	array[x++] = 28;
	array[x++] = 30;
	MPI_Bcast(array, arraysize, MPI_INT, root, MPI_COMM_WORLD);
	//mergesort(array, 0, arraysize, arraysize/2,sorted,my_rank);
	pmerge(array,0,arraysize,calcMid(0,arraysize),sorted);
	
	
	
	
	
	
	
	
	
	
	
	
	if(my_rank==root){
		for (int i=0; i < arraysize; i++){
	//		cout<< array[i] << endl;
		}
	}
	
	MPI_Finalize();

	return 0;
}
