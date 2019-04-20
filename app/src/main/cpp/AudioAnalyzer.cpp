#include <cstdlib>
#include <cmath>
#include <android/log.h>
#include <thread>
#include "AudioAnalyzer.h"


void AudioAnalyzer::startAnalyzer(SoundRecording* recording, int sampleRate) {
    int nSamples = 0.25 * sampleRate; // 0.25s of audio
    mBuffer = new kiss_fft_scalar[nSamples];
    mRecordingInstance = recording;
    mAnalyzerThread = std::thread([=] {
        while(mStarted) {
            int read = mRecordingInstance->read(mBuffer, nSamples);
            __android_log_print(ANDROID_LOG_ERROR, "readThread", "finished reading %d frames", read);
            analyze(mBuffer, read);
        }
    });
}

void AudioAnalyzer::stopAnalyzer() {
    mStarted = false;
    mAnalyzerThread.join();
}


void AudioAnalyzer::analyze(kiss_fft_scalar* samples, int size){
    __android_log_print(ANDROID_LOG_ERROR, __func__, "RUNNING FFT on %d samples", size);
    auto cfg = kiss_fftr_alloc(size, 0, 0, 0);
    auto out = new kiss_fft_cpx[size/2+1];
    auto magnitudes = new float[size/2+1];
    hanningWindow(samples , size);
    kiss_fftr(cfg, samples, out);
    auto pwr = computeMagnitude(out, magnitudes, size/2+1);
    __android_log_print(ANDROID_LOG_ERROR, __func__, "power is %0.6f", pwr);
    free(cfg);
    delete[] out; // should be processed first
}



void AudioAnalyzer::hanningWindow(kiss_fft_scalar* samples, int size)
{
    for (int i = 0; i < size; i++)
        samples[i] *= (0.54-((0.46) * cos((2 * M_PI * i)/size)));
}

float AudioAnalyzer::computeMagnitude(kiss_fft_cpx *in, kiss_fft_scalar *out, int size)
{
    float pwr = 0;
    for (int i =0; i < size; i++) {
        out[i] = pow(in[i].i,2) + pow(in[i].r, 2);
        if (out[i] > 1)
            out[i] = log(out[i]); // how we perceive sound
        pwr += out[i];
    }
    return pwr;
}
