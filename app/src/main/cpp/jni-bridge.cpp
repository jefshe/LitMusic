/*
 * Copyright 2017 The Android Open Source Project
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

#include <jni.h>
#include <android/log.h>

#include "AudioEngine.h"

extern "C" {

static AudioEngine audioEngine;

JNIEXPORT void JNICALL
Java_com_jefshe_litmusic_MainActivity_startEngine(
        JNIEnv *env,
        jobject instance,
        jint deviceId) {

    __android_log_print(ANDROID_LOG_DEBUG, "native-lib", "deviceId: %d", deviceId);
    audioEngine.start(deviceId);
}

JNIEXPORT void JNICALL
Java_com_jefshe_litmusic_MainActivity_setRecording(JNIEnv *env, jobject instance,
                                                                jboolean isRecording) {
    __android_log_print(ANDROID_LOG_DEBUG, "native-lib", "Recording? %d", isRecording);
    audioEngine.setRecording(isRecording);
}

JNIEXPORT void JNICALL
Java_com_jefshe_litmusic_MainActivity_stopEngine(JNIEnv *env, jobject instance) {
    audioEngine.stop();
}

}// End extern "C"
