#ifndef GPIODROID_ANDROIDBUF_H
#define GPIODROID_ANDROIDBUF_H

#include <iostream>
#include <streambuf>
#include <android/log.h>

// Usage:
// std::cout.rdbuf(new androidbuf(LOG_TAG));
//
// Release
// delete std::cout.rdbuf(0)

class androidbuf : public std::streambuf {
public:
    enum { bufsize = 128 }; // ... or some other suitable buffer size
    androidbuf(const std::string& TAG): tag(TAG) {
        this->setp(buffer, buffer + bufsize - 1);
    }

private:
    int overflow(int c)
    {
        if (c == traits_type::eof()) {
            *this->pptr() = traits_type::to_char_type(c);
            this->sbumpc();
        }
        return this->sync()? traits_type::eof(): traits_type::not_eof(c);
    }

    int sync()
    {
        int rc = 0;
        if (this->pbase() != this->pptr()) {
            char writebuf[bufsize+1];
            memcpy(writebuf, this->pbase(), this->pptr() - this->pbase());
            writebuf[this->pptr() - this->pbase()] = '\0';

            rc = __android_log_write(ANDROID_LOG_INFO, tag.c_str(), writebuf) > 0;
            this->setp(buffer, buffer + bufsize - 1);
        }
        return rc;
    }

    char buffer[bufsize];
    std::string tag;
};

#endif //GPIODROID_ANDROIDBUF_H
