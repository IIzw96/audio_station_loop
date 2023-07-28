#ifndef RECORDING_H
#define RECORDING_H

#include <iostream>
#include <vector>
#include <rtaudio/RtAudio.h>
#include <sndfile.h>

const int SAMPLE_RATE = 44100;
const unsigned int FRAMES_PER_BUFFER = 256;

class RecordingProcessor {
public:
    RecordingProcessor(int channels);
    ~RecordingProcessor();

    // These are all possible options we may want. In our final implementation, we can remove them
    // if we decide we don't need them. Added flags in the cpp file to suppress warnings.
    static int audio_callback(void* input_buffer, void* output_buffer,
                            unsigned int buffer_size,
                            double time_info,
                            RtAudioStreamStatus status,
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
    RtAudio* audio_input;
    std::vector<float> audio_buffer;
};

#endif // RECORDING_H
