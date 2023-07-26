#ifndef RECORDING_H
#define RECORDING_H

#include <iostream>
#include <vector>
#include <portaudio.h>
#include <sndfile.h>

const int SAMPLE_RATE = 44100;
const int FRAMES_PER_BUFFER = 256;

class RecordingProcessor {
public:
    RecordingProcessor(int channels);
    ~RecordingProcessor();

    // These are all possible options we may want. In our final implementation, we can remove them
    // if we decide we don't need them. Added flags in the cpp file to suppress warnings.
    static int audio_callback(const void* input_buffer, void* output_buffer,
                            unsigned long frames_per_buffer,
                            const PaStreamCallbackTimeInfo* time_info,
                            PaStreamCallbackFlags status_flags,
                            void* user_data);

    bool get_recording() const;
    void pause();
    void record();
    void resume();
    void stop();
    void store_audio_to_buffer(const float* audio_data, unsigned long num_frames);

private:
    void save_audio_to_file(const std::string& filename);

    bool is_recording;
    int num_channels;
    PaStream* stream;
    std::vector<float> audio_buffer;
};

#endif // RECORDING_H
