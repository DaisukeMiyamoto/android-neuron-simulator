//
// Created by nebula on 7/15/17.
//

#ifndef ANDROID_NEURON_SIMULATOR_BENCHMARK_H
#define ANDROID_NEURON_SIMULATOR_BENCHMARK_H

extern "C" {

#ifdef USE_FLOAT
#define EXP(x) expf((x))
typedef float FLOAT;
#else
#define EXP(x) exp((x))
typedef double FLOAT;
#endif

#define TRIAD_FLOP_PER_STEP 2
#define IZ_FLOP_PER_STEP 14
#define HH_FLOP_PER_STEP 243

double getTime();
void init_array(int size, FLOAT *array, FLOAT value);
void init_array_rand(int size, FLOAT *array, FLOAT max);

double benchmark_daxpy(int max_step, int size);
double benchmark_hh(int max_step, int n_cell);
double benchmark_iz(int max_step, int n_cell);

}
#endif //ANDROID_NEURON_SIMULATOR_BENCHMARK_H
