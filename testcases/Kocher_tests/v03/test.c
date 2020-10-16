#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;

struct timespec time_start, time_end;
size_t time_diff;



static __attribute__ ((noinline)) void leakByteNoinlineFunction(uint8_t k) { 
    temp &= array2[k* 512]; 
}

void victim_fun(size_t x) {
         if (x < array1_size)
                leakByteNoinlineFunction(array1[x]);
}

int main(int argn, char* args[]) {
    int source;

    FILE *file = fopen("temp.txt", "r");

    if (file == NULL) {
        printf("No file!");
        return 0;
    }
    source = fgetc(file);
    victim_fun(source);
    return 0;
}

