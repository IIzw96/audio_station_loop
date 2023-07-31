#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "../constants.h"
#include <iostream>
#include <vector>
#include <RtAudio.h>
#include "../loop/loop.h"

class Playback {
public:
    Playback(int channels);
    ~Playback();

    void play();
    void pause();
    void resume();
    void stop();
    void set_loop();

    void set_volume(float value);
    float get_volume() const;

private:
    bool is_playing;
    int num_channels;
    float volume;

    RtAudio* audio_output;
    static int audio_callback(void* output_buffer, void* input_buffer, unsigned int buffer_size,
                              double time_info, RtAudioStreamStatus status, void* user_data);
};

#endif // PLAYBACK_H