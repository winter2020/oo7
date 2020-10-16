/* EXAMPLE 15:  Pass a pointer to the length */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(size_t *idx) {
    if (*idx < array1_size) {                  
        temp &= array2[array1[*idx] * 512];
    }
}

void main(int argn, char* args[]) {
    size_t source;
    size_t *p = &source;

    FILE *file = fopen("temp.txt", "r");

    if (file == NULL)
        return;

    source = fgetc(file);
    victim_fun(p);
}
