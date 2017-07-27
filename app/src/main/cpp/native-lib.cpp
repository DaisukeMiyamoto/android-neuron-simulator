#include <jni.h>
#include <string>
#include <omp.h>
#include <cpu-features.h>
#include "benchmark.h"

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_checkOMP (
        JNIEnv *env,
        jobject /* this */) {
    char msg_template[1024] = "Max Threads: %d\n";
    char msg_buf[1024];

    sprintf(msg_buf, msg_template, omp_get_num_procs());
    return env->NewStringUTF(msg_buf);
}

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_getCpuFeatures (
        JNIEnv *env,
        jobject /* this */) {
    char msg_buf[2048];

    int cores;
    AndroidCpuFamily family;
    uint64_t features;

    cores = android_getCpuCount();
    family = android_getCpuFamily();
    features = android_getCpuFeatures();

    sprintf(msg_buf, "CPU Cores: %d\n", cores);
    if (family == ANDROID_CPU_FAMILY_ARM) {
        strcat(msg_buf, "Family: ARM\n");
        strcat(msg_buf, "Features: ");
        if (features & ANDROID_CPU_ARM_FEATURE_NEON){
            strcat(msg_buf, "NEON ");
        }
        if (features & ANDROID_CPU_ARM_FEATURE_NEON_FMA){
            strcat(msg_buf, "NEON_FMA ");
        }
        strcat(msg_buf, "\n");
    }
    if (family == ANDROID_CPU_FAMILY_ARM64) {
        strcat(msg_buf, "Family: ARM64\n");
        strcat(msg_buf, "Features: ");
        if (features & ANDROID_CPU_ARM64_FEATURE_ASIMD){
            strcat(msg_buf, "ASIMD ");
        }
        if (features & ANDROID_CPU_ARM64_FEATURE_PMULL){
            strcat(msg_buf, "PMULL ");
        }
        strcat(msg_buf, "\n");
    }
    if(family == ANDROID_CPU_FAMILY_X86) {
        strcat(msg_buf, "Family: x86\n");
    }
    if(family == ANDROID_CPU_FAMILY_X86_64) {
        strcat(msg_buf, "Family: x86_64\n");
    }

    return env->NewStringUTF(msg_buf);
}

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_runBenchmarkDaxpy (
        JNIEnv *env,
        jobject ,
        jint num_threads,
        jint neon_mode) {

    const int N_TRIAL = 50;
    const int N_STEP = 200;
    const int DATA_SIZE = 100000;
    double calc_time = 0.0;
    double mflops;
    char msg_template[1024] = "%d/%d threads: %.2f MFLOPS (%.4f sec)\n";
    char msg_buf[1024];

    if(num_threads==0) { num_threads = omp_get_num_procs(); }
    omp_set_num_threads(num_threads);

    if (neon_mode != 1){
        for (int i=0; i<N_TRIAL; i++) {
            calc_time += benchmark_daxpy(N_STEP, DATA_SIZE);
        }
    }else{
#ifdef HAVE_NEON
        for (int i=0; i<N_TRIAL; i++) {
            calc_time += benchmark_daxpy_neon(N_STEP, DATA_SIZE);
        }
#endif
    }
    mflops = TRIAD_FLOP_PER_STEP * (double)DATA_SIZE * (double)N_STEP * (double)N_TRIAL / calc_time * 0.001 * 0.001;
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

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_runBenchmarkIz (
        JNIEnv *env,
        jobject,
        jint num_threads) {

    const int N_TRIAL = 10;
    const int N_CELL = 10000;
    const int N_STEP = 2000;
    double calc_time = 0.0;
    double mflops;
    char msg_template[1024] = "%d/%d threads: %.2f MFLOPS (%.4f sec)\n";
    char msg_buf[1024];

    if(num_threads==0) { num_threads = omp_get_num_procs(); }
    omp_set_num_threads(num_threads);

    for (int i=0; i<N_TRIAL; i++) {
        calc_time += benchmark_iz(N_STEP, N_CELL);
    }

    mflops = IZ_FLOP_PER_STEP * (double)N_CELL * (double)N_STEP * (double)N_TRIAL / calc_time * 0.001 * 0.001;
    sprintf(msg_buf, msg_template, num_threads, omp_get_num_procs(), mflops, calc_time);

    return env->NewStringUTF(msg_buf);
}


}
