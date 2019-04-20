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

#include "AudioEngine.h"
#include "SoundRecordingUtilities.h"
#include "AudioAnalyzer.h"
#include <aaudio/AAudio.h>
#include <android/log.h>
#include <thread>
#include <mutex>

aaudio_data_callback_result_t recordingDataCallback(
        AAudioStream __unused *stream,
        void *userData,
        void *audioData,
        int32_t numFrames) {

    return ((AudioEngine *) userData)->recordingCallback(
            static_cast<float *>(audioData), numFrames);
}

void errorCallback(AAudioStream __unused *stream,
                   void *userData,
                   aaudio_result_t error){
    if (error == AAUDIO_ERROR_DISCONNECTED){
        // The error callback expects to return immediately so it's not safe to restart our streams
        // in here. Instead we use a separate thread.
        std::function<void(void)> restartFunction = std::bind(&AudioEngine::restart,
                                                              static_cast<AudioEngine *>(userData));
        new std::thread(restartFunction);
    }
}

// Here we declare a new type: StreamBuilder which is a smart pointer to an AAudioStreamBuilder
// with a custom deleter. The function AudioStreamBuilder_delete will be called when the
// object is deleted. Using a smart pointer allows us to avoid memory management of an
// AAudioStreamBuilder.
using StreamBuilder = std::unique_ptr<AAudioStreamBuilder, decltype(&AAudioStreamBuilder_delete)>;

// Now we define a method to construct our StreamBuilder
StreamBuilder makeStreamBuilder(){

    AAudioStreamBuilder *builder = nullptr;
    aaudio_result_t result = AAudio_createStreamBuilder(&builder);
    if (result != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, __func__, "Failed to create stream builder %s (%d)",
              AAudio_convertResultToText(result), result);
        return StreamBuilder(nullptr, &AAudioStreamBuilder_delete);
    }
    return StreamBuilder(builder, &AAudioStreamBuilder_delete);
}

void AudioEngine::start(int deviceId) {
    // store in case we need to restart
    mRecordingDevice = deviceId;

    // Create the recording stream.
    StreamBuilder recordingBuilder = makeStreamBuilder();
    AAudioStreamBuilder_setDeviceId(recordingBuilder.get(), deviceId);
    AAudioStreamBuilder_setDirection(recordingBuilder.get(), AAUDIO_DIRECTION_INPUT);
    AAudioStreamBuilder_setPerformanceMode(recordingBuilder.get(), AAUDIO_PERFORMANCE_MODE_POWER_SAVING);
    AAudioStreamBuilder_setSharingMode(recordingBuilder.get(), AAUDIO_SHARING_MODE_SHARED);
    AAudioStreamBuilder_setFormat(recordingBuilder.get(), AAUDIO_FORMAT_PCM_FLOAT);
    AAudioStreamBuilder_setSampleRate(recordingBuilder.get(), sampleRate);
    AAudioStreamBuilder_setChannelCount(recordingBuilder.get(), kChannelCountMono);
    AAudioStreamBuilder_setDataCallback(recordingBuilder.get(), ::recordingDataCallback, this);
    AAudioStreamBuilder_setErrorCallback(recordingBuilder.get(), ::errorCallback, this);

    aaudio_result_t result = AAudioStreamBuilder_openStream(recordingBuilder.get(), &mRecordingStream);

    if (result != AAUDIO_OK){
        __android_log_print(ANDROID_LOG_DEBUG, __func__,
                            "Error opening recording stream %s",
                            AAudio_convertResultToText(result));
        closeStream(&mRecordingStream);
        return;
    }

    AAudioStream_requestStart(mRecordingStream);
    mAnalyzer.startAnalyzer(&mSoundRecording, sampleRate);
}

void AudioEngine::stop() {
    mAnalyzer.stopAnalyzer();
    stopStream(mRecordingStream);
    closeStream(&mRecordingStream);
}

void AudioEngine::restart(){

    static std::mutex restartingLock;
    if (restartingLock.try_lock()){
        stop();
        start(mRecordingDevice);
        restartingLock.unlock();
    }
}

aaudio_data_callback_result_t AudioEngine::recordingCallback(float *audioData,
                                                             int32_t numFrames) {
    if (mIsRecording)
        mSoundRecording.write(audioData, numFrames);
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}

void AudioEngine::setRecording(bool isRecording) {

    if (isRecording) {
        mSoundRecording.clear();
    }
    mIsRecording = isRecording;
}

void AudioEngine::stopStream(AAudioStream *stream) const {

    static std::mutex stoppingLock;
    stoppingLock.lock();
    if (stream != nullptr) {
        aaudio_result_t result = AAudioStream_requestStop(stream);
        if (result != AAUDIO_OK) {
            __android_log_print(ANDROID_LOG_DEBUG, __func__, "Error stopping stream %s",
                                AAudio_convertResultToText(result));
        }
    }
    stoppingLock.unlock();
}


void AudioEngine::closeStream(AAudioStream **stream) const {

    static std::mutex closingLock;
    closingLock.lock();
    if (*stream != nullptr) {
        aaudio_result_t result = AAudioStream_close(*stream);
        if (result != AAUDIO_OK) {
            __android_log_print(ANDROID_LOG_DEBUG, __func__, "Error closing stream %s",
                                AAudio_convertResultToText(result));
        }
        *stream = nullptr;
    }
    closingLock.unlock();
}
