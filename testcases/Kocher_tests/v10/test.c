/* EXAMPLE 10:  Leak a comparison result. */
/* control dependency */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(int idx, int k) {
    if (idx < array1_size) {                  
        if (array1[idx] == k)
            temp &= array2[0];
    }
}

void main(int argn, char* args[]) {
    int source;
    int source2;

    FILE *file = fopen("temp.txt", "r");

    if (file == NULL)
        return;

    source = fgetc(file);
    source2 = source + 2;
    victim_fun(source, source2);
}
