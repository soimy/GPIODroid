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

gpiod::line::offsets lines2Offsets(JNIEnv *env, jintArray lines) {
    gpiod::line::offsets offsets;
    jsize size = env->GetArrayLength(lines);
    jint* linePtr = env->GetIntArrayElements(lines, nullptr);
    for (jsize i = 0; i < size; i++) {
        offsets.push_back((unsigned int)linePtr[i]);
    }
    env->ReleaseIntArrayElements(lines, linePtr, 0);
    return offsets;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_GPIO_getAllChipsNative(JNIEnv *env, jobject /* thiz */, jobjectArray chip_names) {
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
    chip_names = env->NewObjectArray(totalBank, env->FindClass("java/lang/String"), nullptr);
    for (jsize i = 0; i < chipPaths.size(); i++) {
        env->SetObjectArrayElement(chip_names, i, env->NewStringUTF(chipPaths[i].c_str()));
    }

    return totalBank;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_sym_gpiodroid_GPIO_setLinesNative(
        JNIEnv *env,
        jobject thiz,
        jstring chipName,
        jintArray lines,
        jintArray values) {

    const char* chip_name = env->GetStringUTFChars(chipName, nullptr);
    gpiod::chip chip(chip_name);
    gpiod::line::offsets offsets = lines2Offsets(env, lines);
    gpiod::line::values vals;

    jsize n = env->GetArrayLength(values);
    jint* vPtr = env->GetIntArrayElements(values, nullptr);
    for (jsize i = 0; i < n; i++) {
        vals.push_back(vPtr[i] ? gpiod::line::value::ACTIVE : gpiod::line::value::INACTIVE);
    }

    auto request = chip.prepare_request()
            .set_consumer("GPIODroid")
            .add_line_settings(
                    offsets,
                    gpiod::line_settings()
                        .set_direction(gpiod::line::direction::OUTPUT)
            ).do_request();

    request.set_values(vals);

    return 0;
}

extern "C"
JNIEXPORT jintArray JNICALL
Java_com_sym_gpiodroid_GPIO_getLinesNative(
        JNIEnv *env,
        jobject thiz,
        jstring chipName,
        jintArray lines) {

//    jclass gpioClass = env->GetObjectClass(thiz);
    const char* chip_name = env->GetStringUTFChars(chipName, nullptr);
    gpiod::chip chip(chip_name);
    gpiod::line::offsets offsets = lines2Offsets(env, lines);

    MLOGD << "GPIO line is " << chip_name << " - " << offsets;

    auto request = chip.prepare_request()
            .set_consumer("gpioDroid")
            .add_line_settings(offsets,
                               gpiod::line_settings()
                                    .set_direction(gpiod::line::direction::INPUT))
            .do_request();

    auto vals = request.get_values();
    jintArray ret = env->NewIntArray((jsize)vals.size());
    jint* retPtr = env->GetIntArrayElements(ret, nullptr);
    for (jsize i = 0; i < (jsize)vals.size(); i++) {
        retPtr[i] = vals[i] == gpiod::line::value::ACTIVE ? 1 : 0;
    }

    return ret;
}

