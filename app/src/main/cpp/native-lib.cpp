#include <jni.h>
#include <string>
#include "benchmark.h"

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_runSimulation(
        JNIEnv *env,
        jobject /* this */) {
    std::string msg = "";

    const int LOOP_MAX=100000000;
    benchmark(LOOP_MAX);

    return env->NewStringUTF(msg.c_str());
}

}
