# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.22.1)

# Declares and names the project.

project("gpiodroid")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        gpiodroid

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        gpiodroid.cpp
        AndroidLogger.cpp
        libgpiod/bindings/cxx/chip.cpp
        libgpiod/bindings/cxx/chip-info.cpp
        libgpiod/bindings/cxx/edge-event.cpp
        libgpiod/bindings/cxx/edge-event-buffer.cpp
        libgpiod/bindings/cxx/exception.cpp
        libgpiod/bindings/cxx/info-event.cpp
        libgpiod/bindings/cxx/internal.cpp
        libgpiod/bindings/cxx/line.cpp
        libgpiod/bindings/cxx/line-config.cpp
        libgpiod/bindings/cxx/line-info.cpp
        libgpiod/bindings/cxx/line-request.cpp
        libgpiod/bindings/cxx/line-settings.cpp
        libgpiod/bindings/cxx/misc.cpp
        libgpiod/bindings/cxx/request-builder.cpp
        libgpiod/bindings/cxx/request-config.cpp

        )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

include_directories(${ANDROID_ABI}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libgpiod/bindings/cxx/)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libgpiod/tools)

target_link_libraries( # Specifies the target library.
        gpiodroid

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})