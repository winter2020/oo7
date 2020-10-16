/*  EXAMPLE 9:  Use a separate value to communicate the safety check status. */
/* Somehow a control dependency , but I am going to taint x_is_safe */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

unsigned int array1_size = 16;
uint8_t array1[16];
uint8_t array2[256 * 512];
uint8_t temp = 0;


void victim_fun(size_t x, int *x_is_safe) {
    if (*x_is_safe < array1_size) {                  
        temp &= array2[array1[x] * 512];
    }
}

void main(int argn, char* args[]) {
    int source;
    int *p;

    FILE *file = fopen("temp.txt", "r");

    if (file == NULL)
        return;

    source = fgetc(file);
    p = &source;
    victim_fun(source, p);
}
