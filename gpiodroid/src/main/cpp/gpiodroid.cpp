#include <jni.h>
#include <string>
#include <gpiod.h>
#include <android/log.h>
#include <tools-common.h>

#define LOG_TAG "GPIODroid_native"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static int print_chip_info(const char *path)
{
    struct gpiod_chip_info *info;
    struct gpiod_chip *chip;

    chip = gpiod_chip_open(path);
    if (!chip) {
        LOGE("Unable to open chip '%s'", path);
        return 1;
    }

    info = gpiod_chip_get_info(chip);
    if (!info)
        LOGE("Unable to read info for '%s'", path);

    LOGI("%s [%s] (%zu lines)\n",
         gpiod_chip_info_get_name(info),
         gpiod_chip_info_get_label(info),
         gpiod_chip_info_get_num_lines(info));

    gpiod_chip_info_free(info);
    gpiod_chip_close(chip);

    return 0;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_sym_gpiodroid_gpiod_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_gpiod_getGPIOTotalBank(JNIEnv *env, jobject thiz) {
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
Java_com_sym_gpiodroid_gpiod_setGPIOInfo(JNIEnv *env, jobject thiz, jint bank, jint line,
                                         jint value) {
    // TODO: implement setGPIOInfo()
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_gpiod_getGPIOInfo(JNIEnv *env, jobject thiz, jint bank, jint line) {
    // TODO: implement getGPIOInfo()
}