#include <jni.h>
#include <string>
#include "benchmark.h"

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_runBenchmark (
        JNIEnv *env,
        jobject /* this */) {

    const int N_TRIAL = 1000;
    const int DATA_SIZE = 100000;
    double calc_time = 0.0;
    double mflops = 0.0;
    char msg_template[1024] = "TRIAD: %.2f MFLOPS (%.4f sec)\n";
    char msg_buf[1024];

    for (int i=0; i<N_TRIAL; i++) {
        calc_time += benchmark_triad(DATA_SIZE);
    }

    mflops = TRIAD_FLOP_PER_STEP * (double)DATA_SIZE * (double)N_TRIAL / calc_time * 0.001 * 0.001;
    sprintf(msg_buf, msg_template, mflops, calc_time);

    return env->NewStringUTF(msg_buf);
}

}
