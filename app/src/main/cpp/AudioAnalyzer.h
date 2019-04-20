//
// Created by Theodore Roosevelt on 2019-04-19.
//

#ifndef LITMUSIC_AUDIOANALYZER_H
#define LITMUSIC_AUDIOANALYZER_H



#include "kissfft/kiss_fftr.h"
#include "SoundRecording.h"
class AudioAnalyzer {
public:
    void startAnalyzer(SoundRecording recording, int sampleRate);
    void stopAnalyzer();
private:
    void analyze(int sampleRate, kiss_fft_scalar* samples, int size);
    void hanningWindow(const kiss_fft_scalar* samples, kiss_fft_scalar* copyTo, int size);
    // returns the total power of the signal
    float computeMagnitude(kiss_fft_cpx *in, float *out, int size);
    float* mBuffer;
};


#endif //LITMUSIC_AUDIOANALYZER_H
