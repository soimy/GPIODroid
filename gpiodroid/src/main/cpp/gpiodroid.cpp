#include <jni.h>
#include <string>
#include <cstdlib>
#include <filesystem>
#include <gpiod.hpp>
#include <android/log.h>
#include "Logger.hpp"

#define LOG_TAG "GPIODroid_native"
#define MLOGD Logger(ANDROID_LOG_DEBUG,LOG_TAG)
#define MLOGI Logger(ANDROID_LOG_INFO,LOG_TAG)
#define MLOGE Logger(ANDROID_LOG_ERROR,LOG_TAG)

//#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
//#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
//#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static int print_chip_info(const char *path)
{
    for (const auto& entry: ::std::__fs::filesystem::directory_iterator("/dev/")) {
        if (::gpiod::is_gpiochip_device(entry.path())) {
            ::gpiod::chip chip(entry.path());
            auto info = chip.get_info();

            Logger(ANDROID_LOG_DEBUG, LOG_TAG) << info.name() << " [" <<
                info.label() << "] (" <<
                info.num_lines() << " lines)" << ::std::endl;

        }
    }

    return 0;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_sym_gpiodroid_GPIO_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_GPIO_getGPIOTotalBank(JNIEnv *env, jobject thiz) {
    std::ignore = thiz;
    struct gpiod_chip_info *chipInfo;
    char **paths;
    int totalBank = 0, i, ret = 0;
    totalBank = all_chip_paths(&paths);
    for (i = 0; i < totalBank; i++) {
        if (print_chip_info(paths[i]))
            ret = 1;
        free(paths[i]);
    }
    free(paths);

    return ret > 0 ? -1 : totalBank;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_GPIO_setGPIOInfo(JNIEnv *env, jobject thiz, jint bank, jint line,
                                        jint value) {
    // TODO: implement setGPIOInfo()
    return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_GPIO_getGPIOInfo(JNIEnv *env, jobject thiz, jint bank, jint line) {
    std::ignore = thiz;

    char strbank[32] = {0}, ret[4] = {0};
    sprintf(strbank, "gpiochip%d", bank);

    LOGD("GPIO bank is %d", bank);
    LOGD("GPIO line is %d", line);

}