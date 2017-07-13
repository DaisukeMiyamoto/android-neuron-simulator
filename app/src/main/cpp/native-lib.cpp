#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_nebula_androidneuronsimulator_MainActivity_stringFromRun(
        JNIEnv *env,
        jobject /* this */) {
    std::string msg = "start simulation\n";
    return env->NewStringUTF(msg.c_str());
}
