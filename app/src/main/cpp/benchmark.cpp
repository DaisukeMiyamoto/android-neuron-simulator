//
// Created by nebula on 7/15/17.
//

#include "benchmark.h"

extern "C" {

double benchmark(int loop_max) {
    int i;
    double result;

    for (i = 0; i < loop_max; i++) {
        result += i * 0.001;
    }
    return result;
}

}