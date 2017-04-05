#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//prototype
void allocateRandomArray(long double);
void accessArray(int *, long int);

int main(){
    srand(time(NULL));  // Seed random function
    int i=0;
    for(i=2; i < 32; i++){
        allocateRandomArray(pow(2, i));         //call latency function on arrays of increasing size
    }


}
void allocateRandomArray(long double size){
    int accessSize = (size) / sizeof(int);
    int * randomArray = malloc(accessSize*sizeof(int));
    int counter;

    for(counter=0; counter < accessSize; counter ++){
        randomArray[counter] = counter; 
    }
    for(counter=0; counter < accessSize; counter ++){
        int i,j;
        int swap;
        i = rand() % accessSize;
        j = rand() % accessSize;
        swap = randomArray[i];
        randomArray[i] = randomArray[j];
        randomArray[j] = swap;
    } 

    accessArray(randomArray, accessSize);
    free(randomArray);
}

void accessArray(int *cacheArray, long int size){
    const long double NUM_ACCESSES = 1000000000;
    const int SECONDS_PER_NS = 1000000000;
    int newIndex=0;
    int counter=0;
    int read=0;
    struct timespec startAccess, endAccess;
    long double accessTime = 0;

//    clock_gettime(CLOCK_REALTIME, &startAccess); //start clock
    clock_gettime(CLOCK_REALTIME, &startAccess);
    for(counter = 0; counter < NUM_ACCESSES; counter++){
        newIndex=cacheArray[newIndex];
    }
    clock_gettime(CLOCK_REALTIME, &endAccess);
    //clock_gettime(CLOCK_REALTIME, &endAccess); //end clock

    //calculate the time elapsed in ns per access
    accessTime = (((endAccess.tv_sec - startAccess.tv_sec) * SECONDS_PER_NS) + (endAccess.tv_nsec - startAccess.tv_nsec)) / (NUM_ACCESSES);
    printf("Access time: %Lf for size %ld\n", accessTime, size);
} 
