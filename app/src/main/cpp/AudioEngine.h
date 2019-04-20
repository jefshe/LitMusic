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

#ifndef LITMUSIC_AUDIOENGINE_H
#define LITMUSIC_AUDIOENGINE_H

#include <cstdint>
#include <atomic>
#include <memory>
#include <aaudio/AAudio.h>
#include "SoundRecording.h"
#include "AudioAnalyzer.h"

class AudioEngine {

public:
    void start(int deviceId);
    void stop();
    void restart();
    aaudio_data_callback_result_t recordingCallback(float *audioData, int32_t numFrames);
    void setRecording(bool isRecording);

private:
    std::atomic<bool> mIsRecording = {false};
    SoundRecording mSoundRecording;
    AAudioStream* mRecordingStream = nullptr;
    int mRecordingDevice;
    AudioAnalyzer mAnalyzer;
    void stopStream(AAudioStream *stream) const;
    void closeStream(AAudioStream **stream) const;
};

#endif //LITMUSIC_AUDIOENGINE_H
