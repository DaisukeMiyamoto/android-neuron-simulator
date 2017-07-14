#include <jni.h>
#include <string>

extern "C" {

double benchmark(int loop_max){
    int i;
    double result;

    for (i=0; i<loop_max; i++){
        result += i*0.001;
    }
    return result;
}


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
