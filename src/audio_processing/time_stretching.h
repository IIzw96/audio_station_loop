#ifndef TIME_STRETCHING_H
#define TIME_STRETCHING_H

#include "../constants.h"
#include <vector>
#include <complex>
#include <SoundTouch.h> // Include the SoundTouch library header
#include <fftw3.h>      // Include the FFTW library header

class TimeStretching {
public:
    TimeStretching(float ratio = 1.0, int num_channels = 1, double cutoff_frequency = 4000.);
    ~TimeStretching();

    void set_time_stretch_ratio(float ratio, int channels);
    void process_audio(std::vector<float>& input, std::vector<float>& output);
    void apply_hann_window(std::vector<float>& buffer);
    void overlap_add(std::vector<float>& buffer, std::vector<float>& output);
    void apply_low_pass_filter(std::vector<std::complex<float>>& buffer, double cutoff_freq);
    size_t get_fft_size() const;

private:
    const size_t fft_size = 1024;
    soundtouch::SoundTouch sound_touch;

    int num_channels;          // Number of audio channels (1 for mono, 2 for stereo)
    float stretch_ratio;
    double cutoff_frequency;
    fftw_plan forward_plan; 
    fftw_plan inverse_plan;  

};

#endif // TIME_STRETCHING_H
