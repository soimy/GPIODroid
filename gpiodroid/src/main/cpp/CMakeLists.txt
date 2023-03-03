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
        libgpiod/lib/chip.c
        libgpiod/lib/chip-info.c
        libgpiod/lib/edge-event.c
        libgpiod/lib/info-event.c
        libgpiod/lib/internal.c
        libgpiod/lib/line-config.c
        libgpiod/lib/line-info.c
        libgpiod/lib/line-request.c
        libgpiod/lib/line-settings.c
        libgpiod/lib/misc.c
        libgpiod/lib/request-config.c
        libgpiod/tools/tools-common.c
#        tools-common.c)
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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libgpiod/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libgpiod/tools)

#add_library(gpiod STATIC IMPORTED)
#set_target_properties(gpiod PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/${ANDROID_ABI}/libgpiod.a)

target_link_libraries( # Specifies the target library.
        gpiodroid
#        gpiod

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})