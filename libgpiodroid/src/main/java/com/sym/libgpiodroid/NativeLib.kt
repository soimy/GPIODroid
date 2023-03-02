package com.sym.libgpiodroid

class NativeLib {
    /**
     * A native method that is implemented by the 'libgpiodroid' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String?

    companion object {
        // Used to load the 'libgpiodroid' library on application startup.
        init {
            System.loadLibrary("libgpiodroid")
        }
    }
}