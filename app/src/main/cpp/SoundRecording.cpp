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

#include <android/log.h>
#include "SoundRecording.h"

int32_t SoundRecording::write(const float *sourceData, int32_t numSamples) {
    std::lock_guard<std::mutex> guard(mLock);
    // Check that data will fit, if it doesn't just write as much as we can.
    if (mData.size() + numSamples > kMaxSamples) {
        numSamples = kMaxSamples - mData.size();
        __android_log_print(ANDROID_LOG_ERROR, __func__, "reached max frames!!");
    }
    for (int i = 0; i < numSamples; i++)
        mData.push(sourceData[i]);

    // Notify anyone reading
    mDataAvailable.notify_one();
    return numSamples;
}

int32_t SoundRecording::read(float *targetData, int32_t numSamples){

    std::unique_lock<std::mutex> unique(mLock);
    int32_t framesRead = 0;
    // block if not enough data is available
    if (mData.size() < numSamples)
        mDataAvailable.wait(unique, [=]{ return mData.size() >= numSamples; });
    while (framesRead < numSamples){
        targetData[framesRead++] = mData.front();
        mData.pop();
    }

    return framesRead;
}

void SoundRecording::clear() {
    std::lock_guard<std::mutex> guard(mLock);
    std::queue<float> empty;
    std::swap(mData, empty);
}
