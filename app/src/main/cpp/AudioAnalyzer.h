//
// Created by Theodore Roosevelt on 2019-04-19.
//

#ifndef LITMUSIC_AUDIOANALYZER_H
#define LITMUSIC_AUDIOANALYZER_H


#include <thread>
#include "kissfft/kiss_fftr.h"
#include "SoundRecording.h"
class AudioAnalyzer {
public:
    void startAnalyzer(SoundRecording* recording, int sampleRate);
    void stopAnalyzer();
private:
    void analyze(kiss_fft_scalar* samples, int size);
    void hanningWindow(kiss_fft_scalar* samples, int size);
    // returns the total power of the signal
    float computeMagnitude(kiss_fft_cpx *in, float *out, int size);
    float* mBuffer = nullptr;
    std::thread mAnalyzerThread;
    bool mStarted = true;
    SoundRecording* mRecordingInstance;
};


#endif //LITMUSIC_AUDIOANALYZER_H
