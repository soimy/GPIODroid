package com.sym.gpiodroid

class gpiod {

    /**
     * A native method that is implemented by the 'gpiodroid' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String
    external fun getGPIOTotalBank(): Int
    external fun setGPIOInfo(bank: Int, line: Int, value: Int): Int
    external fun getGPIOInfo(bank: Int, line: Int): Int

    companion object {
        // Used to load the 'gpiodroid' library on application startup.
        init {
            System.loadLibrary("gpiodroid")
        }
    }
}