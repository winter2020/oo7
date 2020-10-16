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


void victim_fun(int idx) {
    if (idx < array1_size) {                  
        temp &= array2[array1[idx] * 512];
    }
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

