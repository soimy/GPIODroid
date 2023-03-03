//
// Created by soimy on 2023/3/3.
//

#ifndef GPIODROID_LOGGER_HPP
#define GPIODROID_LOGGER_HPP

#include <android/log.h>
#include <string>
#include <sstream>

class Logger {

public:
    Logger(const android_LogPriority priority,const std::string& TAG):M_PRIORITY(priority),M_TAG(TAG) {}
    Logger(const Logger& ) = delete;

    ~Logger() {
        logBigMessage(stream.str());
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
        if(message.length()>ANDROID_LOG_BUFF_SIZE) {
            __android_log_print(M_PRIORITY,M_TAG.c_str(),"%s",message.substr(0,ANDROID_LOG_BUFF_SIZE).c_str());
            logBigMessage(message.substr(ANDROID_LOG_BUFF_SIZE));
        } else {
            __android_log_print(M_PRIORITY,M_TAG.c_str(),"%s",message.c_str());
        }
    }

    // the non-member function operator<< will now have access to private members
    template <typename T>

    friend Logger& operator<<(Logger& record, T&& t);
};

template <typename T>

Logger& operator<<(Logger& record, T&& t) {
    record.stream << std::forward<T>(t);
    return record;
}

template <typename T>
Logger& operator<<(Logger&& record, T&& t) {
    return record << std::forward<T>(t);
}

// These are usefully so we don't have to write AndroidLogger(ANDROID_LOG_DEBUG,"MyTAG") every time
static Logger LOGD(const std::string& TAG="NoTag"){
    return Logger(ANDROID_LOG_DEBUG,TAG);
}

static Logger LOGE(const std::string& TAG="NoTag"){
    return Logger(ANDROID_LOG_ERROR,TAG);
}

#endif //GPIODROID_LOGGER_HPP
