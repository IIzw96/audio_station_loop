#ifndef RECORDING_H
#define RECORDING_H

#include <iostream>

class RecordingProcessor {
public:
    RecordingProcessor();
    ~RecordingProcessor();

    void record();
    void pause();
    void stop();

private:
    bool is_recording;
    float current_position_s;
};

#endif // RECORDING_H
