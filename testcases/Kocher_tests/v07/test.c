/* EXAMPLE 7:  Compare against the last known-good value. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(int idx) {
    static size_t last_x = 0;
    if (idx == last_x) {                  
        temp &= array2[array1[idx] * 512];
    }

    if (idx < array1_size)
        last_x = idx;
}

void main(int argn, char* args[]) {
    int source;

    FILE *file = fopen("temp.txt", "r");

    if (file == NULL)
        return;

    source = fgetc(file);
    victim_fun(source);
}
