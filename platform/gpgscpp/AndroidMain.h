/* Copyright (c) 2014 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_MAIN_H
#define ANDROID_MAIN_H

#if defined(Q_OS_ANDROID ) |defined(__ANDROID__)

#include <jni.h>

#include <android/log.h>
#include <functional>

#include "gpg/android_initialization.h"
#include "gpg/android_support.h"

#include "StateManager.h"
#include "SelfDetachingJNIEnv.h"

class AndroidMain {
    public:
        static void OnLoad(JavaVM *vm, void*ptr);
        /**
        * @brief Extensibility purposes.
        * @note
        */
        static std::function<void(JavaVM*,void*)> onLoadCallBack;
        static void NativeOnActivityCreated(JNIEnv* env, jobject thiz, jobject activity, jobject savedInstanceState);
        static void NativeOnActivityResult(JNIEnv* env, jobject thiz, jobject activity, jint request_code, jint result_code, jobject result);
        static void SetAuthResultCallback(JNIEnv * env, jobject thiz, jstring callback);
        /**
         * @brief Returns the current JNI environment.
         * @note This method will cache the JVM reference so future calls do not need to provide it.
         * @param vm JavaVM to load the current environment from
         */
        static SelfDetachingJNIEnv getEnv(JavaVM* vm = nullptr);
        static jclass getActivityClass(JNIEnv* jni, jobject activity = nullptr);
    private:
        static JavaVM *jvm;
        static char *callbackName_;
        static jobject callbackObj_;
        static jmethodID callbackMID_;
        static jclass sActivityClass_;
};

#endif

#endif
