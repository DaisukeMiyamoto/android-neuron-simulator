//
// Created by nebula on 7/15/17.
//

#include <stdlib.h>
#include <omp.h>
#include "benchmark.h"

extern "C" {

void triad_core(int array_size, FLOAT *a, FLOAT *b, FLOAT *c)
{
    FLOAT scalar = 3.141592;
//#pragma omp parallel for
    for (int i=0; i<array_size; i++){
        a[i] = b[i] + scalar * c[i];
    }
}

double benchmark_triad(int size) {
    FLOAT *a;
    FLOAT *b;
    FLOAT *c;
    double start_time, stop_time;

    a = (FLOAT *)malloc(size * sizeof(FLOAT));
    b = (FLOAT *)malloc(size * sizeof(FLOAT));
    c = (FLOAT *)malloc(size * sizeof(FLOAT));
    init_array_rand(size, b, 1.0);
    init_array_rand(size, c, 1.0);

    start_time = getTime();
    triad_core(size, a, b, c);
    stop_time = getTime();

    free(a);
    free(b);
    free(c);
    return(stop_time - start_time);
}

}