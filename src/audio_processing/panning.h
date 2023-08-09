#ifndef PANNING_H
#define PANNING_H

#include "../constants.h"
#include "../database/database.h"

#include <cmath>
#include <iostream>
#include <RtAudio.h>
#include <vector>

struct Stereo {
    float left;
    float right;
};

class Panning {
public:
    Panning(DatabaseManager& db_manager);
    ~Panning();

    int add_loop(const std::string& loop_name, const std::string& loop_length, int loop_bpm);
    bool set_loop(int loop_id);

    void start();
    void stop(); 

private:
    double gain = 0.5;
    double pan = 0.0;  // Initialize to center
    unsigned int phase = 0;
    int selected_loop_id = -1;

    static int rtAudioCallback(void* output_buffer, void* /*input_buffer*/, unsigned int num_frames, double /*stream_time*/, RtAudioStreamStatus /*status*/, void* user_data);

    DatabaseManager& database;
    RtAudio* audio;
    RtAudio::StreamOptions options;
    RtAudio::StreamParameters output_params;
};

#endif // PANNING_H
