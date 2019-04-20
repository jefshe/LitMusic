#include <cstdlib>
#include <cmath>
#include <android/log.h>
#include "AudioAnalyzer.h"

void AudioAnalyzer::analyze(int sampleRate, kiss_fft_scalar* samples, int size){
    __android_log_print(ANDROID_LOG_ERROR, __func__, "RUNNING FFT");

    auto cfg = kiss_fftr_alloc(size, 0, 0, 0);
    auto *cx_in = new kiss_fft_scalar[size];
    auto *cx_out = new kiss_fft_cpx[size/2+1];
    // put `nfft` samples in cx_in[k]
    hanningWindow(samples, cx_in, size);

    // Process the spectrum `cx_out` here: We have `nfft/2+1` (!) samples.
    kiss_fftr(cfg, cx_in, cx_out);

    free(cfg);
    delete[] cx_in;
    delete[] cx_out; // should be processed first
}

void AudioAnalyzer::hanningWindow(const kiss_fft_scalar* samples, kiss_fft_scalar* copy, int size)
{
    for (int i = 0; i < size; i++)
        copy[i] = (kiss_fft_scalar) (samples[i] * (0.54-((0.46) * cos((2 * M_PI * i)/size))));
}
