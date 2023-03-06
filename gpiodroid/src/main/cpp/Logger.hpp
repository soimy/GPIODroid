// Android style Logcat with cpp stream support
// Shen Yiming <soimy@163.com> 2023-03-06
//
// Based on: https://stackoverflow.com/questions/8870174/is-stdcout-usable-in-android-ndk?answertab=modifieddesc#tab-top

#ifndef GPIODROID_LOGGER_HPP
#define GPIODROID_LOGGER_HPP

#include <android/log.h>
#include <string>
#include <sstream>

class Logger {

public:
    Logger(const android_LogPriority priority,const std::string& TAG):M_PRIORITY(priority),M_TAG(TAG) {}
//    Logger(const Logger& ) = delete;

    ~Logger() {
        logBigMessage(stream.str());
    }

    template <typename T> Logger& operator<<(T const& val) {
        stream << val;
        return *this;
    }

private:
    std::stringstream stream;
    const std::string M_TAG;
    const android_LogPriority M_PRIORITY;

    // taken from https://android.googlesource.com/platform/system/core/+/android-2.1_r1/liblog/logd_write.c
    static constexpr const auto ANDROID_LOG_BUFF_SIZE = 1024;

    //Splits debug messages that exceed the android log maximum length into smaller log(s)
    //Recursive declaration
    void logBigMessage(const std::string& message) {
        if(message.length() > ANDROID_LOG_BUFF_SIZE) {
            __android_log_print(M_PRIORITY,M_TAG.c_str(),"%s",message.substr(0,ANDROID_LOG_BUFF_SIZE).c_str());
            logBigMessage(message.substr(ANDROID_LOG_BUFF_SIZE));
        } else {
            __android_log_print(M_PRIORITY,M_TAG.c_str(),"%s",message.c_str());
        }
    }

};


#endif //GPIODROID_LOGGER_HPP
