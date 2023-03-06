#include <jni.h>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <gpiod.hpp>
#include <android/log.h>
#include "Logger.hpp"

#define LOG_TAG "GPIODroid_native"
#define MLOGD Logger(ANDROID_LOG_DEBUG,LOG_TAG)
//#define MLOGI Logger(ANDROID_LOG_INFO,LOG_TAG)
//#define MLOGE Logger(ANDROID_LOG_ERROR,LOG_TAG)


extern "C" JNIEXPORT jstring JNICALL
Java_com_sym_gpiodroid_GPIO_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_GPIO_getGPIOTotalBank(JNIEnv *env, jobject thiz, jobjectArray chip_names) {
    std::ignore = thiz;
    int totalBank = 0;
    std::vector<std::string> chipPaths;
    for (const auto& entry: ::std::__fs::filesystem::directory_iterator("/dev/")) {
        if (::gpiod::is_gpiochip_device(entry.path())) {
            ::gpiod::chip chip(entry.path());
            auto info = chip.get_info();

            chipPaths.push_back(entry.path().string());
            totalBank ++;
            MLOGD << info.name() << " [" <<
                  info.label() << "] (" <<
                  info.num_lines() << " lines)";

        }
    }
    chip_names = env->NewObjectArray(totalBank, env->FindClass("java/lang/String"), NULL);
    for (jsize i = 0; i < chipPaths.size(); i++) {
        env->SetObjectArrayElement(chip_names, i, env->NewStringUTF(chipPaths[i].c_str()));
    }

    return totalBank;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_GPIO_setGPIOInfo(JNIEnv *env, jobject thiz, jint bank, jint line, jint value) {
    // TODO: implement setGPIOInfo()
    return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_GPIO_getGPIOInfo(JNIEnv *env, jobject thiz, jint bank, jint line) {
    // TODO: implement setGPIOInfo()
    std::ignore = thiz;

//    char strbank[32] = {0}, ret[4] = {0};
//    sprintf(strbank, "gpiochip%d", bank);

    MLOGD
        << "GPIO bank is " << bank << "\n"
        << "GPIO line is " << line;


}