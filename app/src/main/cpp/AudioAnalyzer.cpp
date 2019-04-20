#include <cstdlib>
#include <cmath>
#include <android/log.h>
#include <thread>
#include "AudioAnalyzer.h"


void AudioAnalyzer::startAnalyzer(SoundRecording recording, int sampleRate) {
    int nSamples = 0.25 * sampleRate; // 0.25s of audio
    mBuffer = new kiss_fft_scalar[nSamples];
    std::thread()
}



void AudioAnalyzer::analyze(int sampleRate, kiss_fft_scalar* samples, int size){
    __android_log_print(ANDROID_LOG_ERROR, __func__, "RUNNING FFT");

    auto cfg = kiss_fftr_alloc(size, 0, 0, 0);
    auto in = new kiss_fft_scalar[size];
    auto out = new kiss_fft_cpx[size/2+1];
    auto magnitudes = new float[size/2+1];
    hanningWindow(samples, in, size);
    kiss_fftr(cfg, in, out);
    computeMagnitude(out, magnitudes, size/2+1);
    free(cfg);
    delete[] in;
    delete[] out; // should be processed first
}



void AudioAnalyzer::hanningWindow(const kiss_fft_scalar* samples, kiss_fft_scalar* copy, int size)
{
    for (int i = 0; i < size; i++)
        copy[i] = (kiss_fft_scalar) (samples[i] * (0.54-((0.46) * cos((2 * M_PI * i)/size))));
}

float AudioAnalyzer::computeMagnitude(kiss_fft_cpx *in, kiss_fft_scalar *out, int size)
{
    for

}
