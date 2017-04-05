#include <stdio.h> 
#include <stdlib.h>
#include <time.h>

/*
 * This test is affected greatly by the number of processes using the cache at the runtime.
 * I suggest closing as much processes as possible to get accurate results.
 */

#define KB 1024
#define MB 1024*1024

int main() {
    unsigned int strides = 512 * 1024 * 1024; //for better timing, This is larger(shows more different). 
    static int arr[4 * 1024 * 1024]; // 4MB of data larager than L1 and L2 cache
    unsigned int Mod;
    unsigned int i;
    int s;

    int sizes[] = {1*KB, 4*KB, 8*KB, 16*KB, 32*KB, 64*KB, 128*KB, 256*KB, 512*KB, 1*MB, 2*MB, 3*MB, 4*MB};
    
    int sizes_len = sizeof(sizes)/sizeof(int);

    clock_t start;
    double elapsed;

    // Do the test for mutible different sizes stored in lengthMod.
    for (s = 0; s < sizes_len; s++) {
	 Mod = sizes[s] - 1; 
	 //time accessing the memory and incrementing the vallue accessed. It will be accessed "strides" times for better timing. 
	 start = clock();
	 for (i = 0; i < strides; i++) {
	    arr[(i * 16) & Mod]++; // & does the same as module. Addessed is jumping by 16*i.
	 }

	 elapsed = (double)(clock() - start) / CLOCKS_PER_SEC; // calculate the difference time since start.
	 printf("%d KB, %.9f \n", sizes[s] / 1024, elapsed);

    }
    printf("The first jump in the timing is where L1 cache reached its limmit\n");
    return 0;
}
