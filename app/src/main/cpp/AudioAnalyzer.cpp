#include <stdlib.h>
#include <android/log.h>
#include "AudioAnalyzer.h"
#include "kissfft/kiss_fftr.h"


void AudioAnalyzer::analyze(int sampleRate){
    int nfft = 0.5 * sampleRate; // 500ms of audio

    __android_log_print(ANDROID_LOG_ERROR, __func__, "RUNNING FFT");
    kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0, 0, 0);
    kiss_fft_scalar *cx_in = new kiss_fft_scalar[nfft];
    kiss_fft_cpx *cx_out = new kiss_fft_cpx[nfft/2+1];

// put `nfft` samples in cx_in[k]

    kiss_fftr(cfg, cx_in, cx_out);

// Process the spectrum `cx_out` here: We have `nfft/2+1` (!) samples.

    free(cfg);
    delete[] cx_in;
    delete[] cx_out;
}
