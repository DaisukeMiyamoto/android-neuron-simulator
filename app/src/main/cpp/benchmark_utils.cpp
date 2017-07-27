//
// Created by nebula on 7/15/17.
//

#include <stdlib.h>
#include <time.h>
#include "benchmark.h"

extern "C" {

double getTime()
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return(ts.tv_sec + ts.tv_nsec*1e-9);
}

void init_array(int size, FLOAT *array, FLOAT value) {
    for (int i = 0; i < size; i++) { array[i] = value; }
}

void init_rand() {
    srand((unsigned)time(NULL));
}
void init_array_rand(int size, FLOAT *array, FLOAT max) {
    for (int i = 0; i < size; i++) { array[i] = ((double)rand() / ((double)RAND_MAX + 1)) * max; }
}

}