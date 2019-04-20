//
// Created by Theodore Roosevelt on 2019-04-19.
//

#ifndef LITMUSIC_AUDIOANALYZER_H
#define LITMUSIC_AUDIOANALYZER_H



#include "kissfft/kiss_fftr.h"
class AudioAnalyzer {
public:
    void analyze(int sampleRate, kiss_fft_scalar* samples, int size);
private:
    void hanningWindow(const kiss_fft_scalar* samples, kiss_fft_scalar* copyTo, int size);
};


#endif //LITMUSIC_AUDIOANALYZER_H
