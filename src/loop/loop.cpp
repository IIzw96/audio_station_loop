#include "loop.h"

LoopControls::LoopControls() {
    // Constructor
    is_playing = false;
    tempo_ratio = 1.0f; // Normal tempo
    key = 0; // Default key set to 0 (no pitch shift)
}

LoopControls::~LoopControls() {
    // Destructor
}

void LoopControls::play() {
    // More logic needed beyond this point
    is_playing = true;
    std::cout << "Loop playback started." << std::endl;
}

void LoopControls::pause() {
    is_playing = false;
    std::cout << "Loop playback paused." << std::endl;
}

void LoopControls::stop() {
    is_playing = false;
    std::cout << "Loop playback stopped." << std::endl;
}

void LoopControls::set_tempo(float tempo_ratio) {
    this->tempo_ratio = tempo_ratio;
    std::cout << "Loop tempo set to: " << tempo_ratio << std::endl;
}

void LoopControls::set_key(int key) {
    this->key = key;
    std::cout << "Loop key set to: " << key << std::endl;
}
