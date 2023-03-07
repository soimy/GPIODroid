package com.sym.gpiodroid

import android.app.Activity
import java.lang.ref.WeakReference

class GPIO {

    /**
     * A native method that is implemented by the 'gpiodroid' native library,
     * which is packaged with this application.
     */
    private external fun getAllChipsNative(chipNames: Array<String>): Int
    external fun setLinesNative(chipIds: Array<Int>, line: Int, value: Int): Int
    external fun getLinesNative(chipPath: String, line: Int): Int

    companion object {
        private lateinit var UnityActivityRef: WeakReference<Activity>

        fun recieveUnityActivity(tActivity: Activity) {
            UnityActivityRef = WeakReference<Activity>(tActivity)
        }
        // Used to load the 'gpiodroid' library on application startup.
        init {
            System.loadLibrary("gpiodroid")
        }

        private lateinit var _chipNames: Array<String>
    }

    fun gpioGetTotalBank(): Int {
        return getAllChipsNative(_chipNames)
    }

}