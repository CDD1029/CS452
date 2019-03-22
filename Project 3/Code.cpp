#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

// New compile and run commands for MPI!
// mpicxx -o blah file.cpp
// mpirun -q -np 32 blah
void mergesort(int * a, int first, int last, int * sorted, int my_rank);
void smerge(int * a, int first1, int last1, int first2, int last2, int * sorted);
int calcRank(int * a, int first, int last, int valtofind);
void pmerge(int * a, int first, int last, int mid, int * sorted);
int calcMid(int first, int last){
    return first + (last - first) / 2;
}
int calcFShapey(int logn, int size,int iteration) {
    int toSend = logn * iteration;
    if(toSend >= size) return -1; //error
    return toSend;
}
int calcLShapey(int logn, int size,int iteration) {
    int toSend = logn * (iteration + 1);
    if(toSend >= size) return size;
    return toSend;
}
void fillFirstandLast(int* first, int* last, int* iteration, int size, int logn){
    *first = calcFShapey(logn,size,*iteration);
    *last = calcLShapey(logn,size,*iteration);
    *iteration += 1;
}
void SHAPESYAY(int * shapeyshape, int numberOfShapeyShapes, int logn, int logm, int size, int * srank) {
    cout << "sahpes " << size << " - " << logn << endl;
    for(int i = 0; i < 4; i++) cout << srank[i] << endl;
    int i = 0; int iteration = 0;int first = 0;int last = 0; int shapeyIndex = 0; int prevSrank = -1;
    fillFirstandLast(&first,&last,&iteration,size,logn);
    while(shapeyIndex < numberOfShapeyShapes){
        int toCheck = srank[i];
        cout << first << " -- " << last << "--" << toCheck << endl;
        if(toCheck <= last) {
            bool sameConsecutiveSranks = false;
            if(i > 0) sameConsecutiveSranks = srank[i] == srank[i-1];
            if(toCheck < first || sameConsecutiveSranks) {
                shapeyshape[shapeyIndex++] = 0;
            }
            else if(toCheck == first){
                shapeyshape[shapeyIndex++] = 1;
                first = first + 1;
            }
            else if(toCheck > first){
                shapeyshape[shapeyIndex++] = toCheck - first; // the +1 that normally is needed is negated
                first = toCheck;
            }
            i++;
            if(i >= logm) {
                do{
                    shapeyshape[shapeyIndex++] = last - first + 1;
                    fillFirstandLast(&first,&last,&iteration,size,logn);
                }
                while(shapeyIndex < numberOfShapeyShapes);
            }
        }
        else{
            shapeyshape[shapeyIndex++] = last - first+1;
            fillFirstandLast(&first,&last,&iteration,size,logn);
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
int calcRank(int * a, int first, int last, int valtofind){
    cout << valtofind << " ahhh " << first << " ahhh " << last << endl;
    for(int i = first; i <= last; i++) cout << a[i] << endl;
    if(first==last)
        return first;
    if (first>last)
        return calcRank(a, last, first, valtofind);
    if (first + 1 == last){
        if (a[first]>=valtofind)
            return first;
        cout << "returning " << last << endl;
        return last;
    }
    int m = calcMid(first,last);

    if (valtofind<a[m]){
        int z = calcRank(a, first, m, valtofind);
        cout << "returnign " << z << endl;
        return z;}
    if (valtofind>a[m])
        return calcRank(a, m, last, valtofind);
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
    int toReturn = 0;
    if (a <= 0) return -1;// or raise error but this isn't production code... this isnt even a proper log function
    while(a > 1){
        toReturn++;
        a/=2;
    }
    return toReturn;
}
void pmerge(int * a, int first, int last, int mid, int * sorted){
    int * left = &a[first];
    int * right = &a[mid];
    int leftsize = mid - first;
    int rightsize = last - mid;
    int leftchunk = log(leftsize);
    int rightchunk = log(rightsize);
    int totalChunks = leftchunk + rightchunk;
    int lranksize = leftsize/leftchunk;
    int * leftranks = new int[leftchunk];
    int rranksize = rightsize/rightchunk;
    int * rightranks = new int[rightchunk];
    int leftindex = 0;
    int rightindex = 0;
    int * shapeyshapesf = new int[leftchunk + rightchunk]; // remember to delete our pointers
    int * shapeyshapesf2 = new int[totalChunks]; // remember to delete our pointers
    cout << leftchunk << "asd"<<endl;
    while(leftindex<leftchunk){
        leftranks[leftindex]= calcRank(right, 0, rightsize - 1, left[leftindex*leftchunk]);
        cout << leftranks[leftindex] << "received at index " << leftindex << endl;
        leftindex += 1;
    }
    while(rightindex<rightchunk){
        rightranks[rightindex]=calcRank(left, 0, leftsize - 1, right[rightindex*rightchunk]);
        cout << rightranks[rightindex] << "r"<< endl;

        rightindex++;
    }
//void SHAPESYAY(int * shapeyshape, int numberOfShapeyShapes, int logn, int size, int * srank) {

    SHAPESYAY(shapeyshapesf,8,4,16,rightranks);
    SHAPESYAY(shapeyshapesf2,8,4,16,leftranks);
    for(int i = 0; i < totalChunks; i++)
        cout << shapeyshapesf[i] << endl;
    cout << "break" << endl;
    for(int i = 0; i < totalChunks; i++)
        cout << shapeyshapesf2[i] << endl;

}
int main (int argc, char * argv[]) {
    int arraysize;

    arraysize = 32;
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

    //mergesort(array, 0, arraysize, arraysize/2,sorted,my_rank);
    pmerge(array,0,arraysize,8,sorted);













    return 0;
}
