#include <jni.h>
#include <string>

extern "C" {

JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MyTask_runSimulation(
        JNIEnv *env,
        jobject /* this */) {
    std::string msg = "calculating\n";
    return env->NewStringUTF(msg.c_str());
}

}
