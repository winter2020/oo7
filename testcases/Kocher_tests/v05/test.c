/*  EXAMPLE 5:  Use x as the initial value in a for() loop. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(int idx) {
    size_t i;
    if (idx < array1_size) {                  
        for (i = idx - 1; i >= 0; i--) 
            temp &= array2[array1[idx] * 512];
    }
}

void main(int argn, char* args[]) {
    int source;

    FILE *file = fopen("temp.txt", "r");

    if (file == NULL)
        return;

    source = fgetc(file);
    victim_fun(source);
}
