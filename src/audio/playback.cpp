#include "playback.h"

Playback::Playback() {
    is_playing = false;
    volume = 1.0f;      // Default volume 100%
}

Playback::~Playback() {
    // Destructor
}

void Playback::play() {
    // We will have to implement further logic from here on
    is_playing = true;
    std::cout << "Playback started." << std::endl;
}

void Playback::pause() {
    is_playing = false;
    std::cout << "Playback paused." << std::endl;
}

void Playback::stop() {
    is_playing = false;
    std::cout << "Playback stopped." << std::endl;
}
