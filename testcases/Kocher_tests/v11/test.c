/* EXAMPLE 11:  Use memcmp() to read the memory for the leak. */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(int idx) {
    if (idx < array1_size) {                  
        temp = memcmp(&temp, array2 + (array1[idx] * 512), 1);
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
