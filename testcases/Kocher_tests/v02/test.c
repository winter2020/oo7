/* EXAMPLE 2:  Moving the leak to a local function that can be inlined. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*  Moving the leak to a local inlined function. */

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


inline static void leak_byte_local_function(uint8_t k) {
    temp &= array2[(k)* 512]; 
}

void victim_fun(int idx) {
    if (idx < array1_size) {                  
        leak_byte_local_function(array1[idx]);
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
