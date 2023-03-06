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
Java_com_sym_gpiodroid_GPIO_getGPIOAllChips(JNIEnv *env, jobject /* thiz */, jobjectArray chip_names) {
    int totalBank = 0;
    std::vector<std::string> chipPaths;
    for (const auto& entry: ::std::filesystem::directory_iterator("/dev/")) {
        if (::gpiod::is_gpiochip_device(entry.path())) {
            ::gpiod::chip chip(entry.path());
            auto info = chip.get_info();

            chipPaths.push_back(info.name());
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
Java_com_sym_gpiodroid_GPIO_setGPIOInfo(JNIEnv *env, jobject /* thiz */, jint bank, jint line, jint value) {
    // TODO: implement setGPIOInfo()
    return 0;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_GPIO_getGPIOInfo(JNIEnv *env, jobject /* thiz */, jstring chipPath, jint line) {
    const char* chip_name = env->GetStringUTFChars(chipPath, nullptr);
    gpiod::line::offset offset = line;
    gpiod::chip chip(chip_name);

//    char strbank[32] = {0}, ret[4] = {0};
//    sprintf(strbank, "gpiochip%d", bank);

    MLOGD << "GPIO line is " << chip_name << " - " << offset;

    auto request = chip.prepare_request()
            .set_consumer("gpioDroid")
            .add_line_settings(offset, gpiod::line_settings().set_direction(gpiod::line::direction::INPUT))
            .do_request();
    auto val = request.get_value(offset);

    return val == gpiod::line::value::ACTIVE ? 1 : 0;
}