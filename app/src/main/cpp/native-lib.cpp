#include <jni.h>
#include <string>
#include <omp.h>
#include "benchmark.h"

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_checkOMP (
        JNIEnv *env,
        jobject /* this */) {
    char msg_template[1024] = "Max Threads=%d\n";
    char msg_buf[1024];

    sprintf(msg_buf, msg_template, omp_get_num_procs());
    return env->NewStringUTF(msg_buf);
}

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_runBenchmarkTriad (
        JNIEnv *env,
        jobject ,
        jint num_threads) {

    const int N_TRIAL = 1000;
    const int DATA_SIZE = 100000;
    double calc_time = 0.0;
    double mflops;
    char msg_template[1024] = "%d/%d threads: %.2f MFLOPS (%.4f sec)\n";
    char msg_buf[1024];

    if(num_threads==0) { num_threads = omp_get_num_procs(); }
    omp_set_num_threads(num_threads);

    for (int i=0; i<N_TRIAL; i++) {
        calc_time += benchmark_triad(DATA_SIZE);
    }
    mflops = TRIAD_FLOP_PER_STEP * (double)DATA_SIZE * (double)N_TRIAL / calc_time * 0.001 * 0.001;
    sprintf(msg_buf, msg_template, num_threads, omp_get_num_procs(), mflops, calc_time);

    return env->NewStringUTF(msg_buf);
}

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_runBenchmarkHH (
        JNIEnv *env,
        jobject,
        jint num_threads) {

    const int N_TRIAL = 10;
    const int N_CELL = 1000;
    const int N_STEP = 2000;
    double calc_time = 0.0;
    double mflops;
    char msg_template[1024] = "%d/%d threads: %.2f MFLOPS (%.4f sec)\n";
    char msg_buf[1024];

    if(num_threads==0) { num_threads = omp_get_num_procs(); }
    omp_set_num_threads(num_threads);

    for (int i=0; i<N_TRIAL; i++) {
        calc_time += benchmark_hh(N_STEP, N_CELL);
    }

    mflops = HH_FLOP_PER_STEP * (double)N_CELL * (double)N_STEP * (double)N_TRIAL / calc_time * 0.001 * 0.001;
    sprintf(msg_buf, msg_template, num_threads, omp_get_num_procs(), mflops, calc_time);

    return env->NewStringUTF(msg_buf);
}

}
