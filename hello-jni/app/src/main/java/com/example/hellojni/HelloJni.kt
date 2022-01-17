/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.example.hellojni

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.example.hellojni.databinding.ActivityHelloJniBinding

class HelloJni : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        /*
         * Retrieve our TextView and set its content.
         * the text is retrieved by calling a native
         * function.
         */
        val binding = ActivityHelloJniBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.helloTextview.text = stringFromJNI()
    }

    fun init() {
        try {
            val cls: java.lang.Class<*> = java.lang.Class.forName("android.webkit.WebViewDelegate")
            webviewDelegate = cls.getDeclaredConstructor().newInstance()
            callDrawGLFunction = cls.getMethod("drawWebViewFunctor", Canvas::class.java, Int::class.javaPrimitiveType)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                val cls2: java.lang.Class<*> =
                    WebView.getWebViewClassLoader().loadClass("com.android.webview.chromium.DrawFunctor")
                val method: java.lang.reflect.Method = cls2.getMethod("nativeGetFunctionTable")
                method.setAccessible(true)
                val functionTablePointer = method.invoke(cls2) as Long
                DrawGLFunctor.aw_setDrawFnFunctionTable(functionTablePointer)
                Logger.MAIN.e("InvokeFunctorCompactV31 functionTablePointer=$functionTablePointer")
            }
        } catch (e: java.lang.Exception) {
            Logger.MAIN.e("InvokeFunctorCompactV31 init fail", e)
            webviewDelegate = null
            callDrawGLFunction = null
            invokeDrawFunctor = null
        }
    }

    fun callDrawGLFunction(canvas: Canvas?, nativeDrawGLFunctor: Long) {
        if (webviewDelegate == null) {
            return
        }
        try {
            val drawGLFunctor: Int = DrawGLFunctor.aw_getFunctorHandle(nativeDrawGLFunctor)
            callDrawGLFunction.invoke(webviewDelegate, canvas, drawGLFunctor)
        } catch (e: java.lang.Exception) {
            Logger.MAIN.e(e)
        }
    }

    /*
     * A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
    external fun stringFromJNI(): String?

    /*
     * This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
    external fun unimplementedStringFromJNI(): String?

    companion object {
    /*
     * this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.hellojni/lib/libhello-jni.so
     * at the installation time by the package manager.
     */
        init {
            System.loadLibrary("hello-jni")
        }
    }
}

