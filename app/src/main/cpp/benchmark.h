//
// Created by nebula on 7/15/17.
//

#ifndef ANDROID_NEURON_SIMULATOR_BENCHMARK_H
#define ANDROID_NEURON_SIMULATOR_BENCHMARK_H

extern "C" {

#define TRIAD_FLOP_PER_STEP 2
#define HH_FLOP_PER_STEP 243

double benchmark_triad(int size);
//double benchmark_hh(int max_step, int n_cell);

}
#endif //ANDROID_NEURON_SIMULATOR_BENCHMARK_H
