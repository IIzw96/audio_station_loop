#include "time_stretching.h"

TimeStretching::TimeStretching(float ratio, int num_channels, double cutoff_freq) : num_channels(num_channels), cutoff_frequency(cutoff_freq) {
    // Constructor
    sound_touch.setSampleRate(44100); // Set your audio sample rate
    set_time_stretch_ratio(ratio, num_channels);
    // Create FFTW plans for forward and inverse transforms
    forward_plan = fftw_plan_dft_1d(get_fft_size(), nullptr, nullptr, FFTW_FORWARD, FFTW_ESTIMATE);
    inverse_plan = fftw_plan_dft_1d(get_fft_size(), nullptr, nullptr, FFTW_BACKWARD, FFTW_ESTIMATE);
}

TimeStretching::~TimeStretching() {
    // Destructor
    fftw_destroy_plan(forward_plan);
    fftw_destroy_plan(inverse_plan);
}

void TimeStretching::set_time_stretch_ratio(float ratio, int channels) {
    // Set time stretch ratio and the number of channels to apply it to
    sound_touch.setRate(ratio);
    sound_touch.setChannels(channels);  // Set the number of audio channels (1 for mono, 2 for stereo)
}

void TimeStretching::apply_hann_window(std::vector<float>& buffer) {
    // This is a pre-processing step before applying the fourier transform
    const size_t N = buffer.size();
    for (size_t i = 0; i < N; ++i) {
        double window = 0.5 * (1.0 - std::cos(2.0 * PI * i / (N - 1)));
        buffer[i] *= window;
    }
}

void TimeStretching::apply_low_pass_filter(std::vector<std::complex<float>>& buffer, double cutoff_freq) {
    const double sample_rate = 44100.0;     // This seems to be a common sample rate
    const double nyquist = sample_rate / 2.0;
    const double omega_c = 2.0 * PI * cutoff_freq / nyquist;

    // Moving average low-pass filter
    double filter_gain = 0.0;   // Normalizes filter
    std::vector<double> filter_coefficients(get_fft_size());
    for (size_t i = 0; i < get_fft_size(); ++i) {
        double sinc = (i == 0) ? 1.0 : std::sin(omega_c * i) / (PI * i);
        filter_coefficients[i] = sinc;
        filter_gain += filter_coefficients[i];
    }
    filter_gain = 1.0 / filter_gain;

    // Apply the filter to the buffer
    for (size_t i = 0; i < std::min(buffer.size(), filter_coefficients.size()); ++i) {
    buffer[i] *= filter_coefficients[i] * filter_gain;
}
}

void TimeStretching::overlap_add(std::vector<float>& buffer, std::vector<float>& output) {
    const size_t window_size = buffer.size() / 4;
    const size_t bin_size = window_size / 2;

    // Calculate the number of frames
    size_t num_frames = (buffer.size() + window_size - 1) / window_size;

    // Calculate the size of the output vector
    size_t output_size = (num_frames - 1) * bin_size + window_size;

    // Resize the output vector and initialize it with zeros
    output.resize(output_size, 0.0);

    // Overlap and add frames, this should ultimately be smaller than buffer
    for (size_t i = 0; i < num_frames; ++i) {
        size_t start_idx = i * bin_size;
        size_t end_idx = std::min(start_idx + window_size, buffer.size());
        size_t output_start_idx = i * bin_size;

        for (size_t j = start_idx, k = 0; j < end_idx; ++j, ++k) {
            output[output_start_idx + k] += buffer[j];
        }
    }
}

void TimeStretching::process_audio(std::vector<float>& input, std::vector<float>& output) {
    apply_hann_window(input);   // Smooths signal at the edges

    // Stretch the audio
    std::vector<float> stretched_output(input.size() * this -> stretch_ratio);
    sound_touch.putSamples(input.data(), static_cast<uint>(input.size())); // Provide the input samples to SoundTouch
    sound_touch.receiveSamples(stretched_output.data(), static_cast<uint>(stretched_output.size()));
 
    // Apply FFT to the stretched output
    std::vector<std::complex<float>> complex_buffer(get_fft_size());
    for (size_t i = 0; i < get_fft_size(); ++i) {
        complex_buffer[i] = std::complex<double>(stretched_output[i], 0.0);
    }

    fftw_execute_dft(forward_plan, reinterpret_cast<fftw_complex*>(complex_buffer.data()), reinterpret_cast<fftw_complex*>(complex_buffer.data()));

    // Attenuate signals above the cutoff_frequency
    apply_low_pass_filter(complex_buffer, this -> cutoff_frequency);

    // Apply inverse FFT
    fftw_execute_dft(inverse_plan, reinterpret_cast<fftw_complex*>(complex_buffer.data()), reinterpret_cast<fftw_complex*>(complex_buffer.data()));

    // Convert to real values
    for (size_t i = 0; i < get_fft_size(); ++i) {
        stretched_output[i] = std::real(complex_buffer[i]) / get_fft_size();
    }

    // Stitch the frames back seemlessly
    overlap_add(stretched_output, output);
}

size_t TimeStretching::get_fft_size() const {
    return fft_size;
}
