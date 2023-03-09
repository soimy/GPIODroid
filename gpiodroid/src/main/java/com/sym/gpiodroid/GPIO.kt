package com.sym.gpiodroid

import android.app.Activity
import java.lang.ref.WeakReference
import kotlin.concurrent.thread

class GPIO {

    /**
     * A native method that is implemented by the 'gpiodroid' native library,
     * which is packaged with this application.
     */
    private external fun getAllChipsNative(): Array<String>
    private external fun setLinesNative(chipName: String, lines: IntArray, values: IntArray): Int
    private external fun getLinesNative(chipName: String, lines: IntArray): IntArray
    private external fun edgeDetectsNative(chipName: String, lines: IntArray): Int

    private var worker: Thread = Thread {
        // TODO
    }


    companion object {
        private lateinit var UnityActivityRef: WeakReference<Activity>

        fun recieveUnityActivity(tActivity: Activity) {
            UnityActivityRef = WeakReference<Activity>(tActivity)
        }

        private external fun nativeInit()

        // Used to load the 'gpiodroid' library on application startup.
        init {
            System.loadLibrary("gpiodroid")
            nativeInit()
        }

        private lateinit var chipPaths: Array<String>
    }

    fun gpioGetTotalBank(): Int {
        chipPaths = getAllChipsNative()
        return chipPaths.size;
    }

    fun addEdgeDetectionCallback(): Unit {
        worker.isDaemon = true;
        worker.start();
    }

}