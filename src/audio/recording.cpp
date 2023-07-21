#include "recording.h"

RecordingProcessor::RecordingProcessor() {
    // Constructor
    is_recording = false;
    current_position_s = 0.0f;
}

RecordingProcessor::~RecordingProcessor() {
    // Destructor
}

void RecordingProcessor::record() {
    // We will need to update the logic below this point
    is_recording = true;
    std::cout << "Recording started." << std::endl;
}

void RecordingProcessor::pause() {
    is_recording = false;
    std::cout << "Recording paused." << std::endl;
}

void RecordingProcessor::stop() {
    is_recording = false;
    std::cout << "Recording stopped." << std::endl;
}
