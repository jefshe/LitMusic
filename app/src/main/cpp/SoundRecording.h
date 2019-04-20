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

#ifndef LITMUSIC_SAMPLE_H
#define LITMUSIC_SAMPLE_H

#include <cstdint>
#include <array>
#include <atomic>
#include <mutex>
#include <queue>

#include "Definitions.h"

constexpr int kMaxSamples = 480000; // 10s of audio data @ 48kHz

class SoundRecording {

public:
    int32_t write(const float *sourceData, int32_t numSamples);
    int32_t read(float *targetData, int32_t numSamples);
    void clear();
    static const int32_t getMaxSamples() { return kMaxSamples; };

private:
    std::mutex mLock;
    std::queue<float> mData;
};

#endif //LITMUSIC_SAMPLE_H
