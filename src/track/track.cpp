#include "track.h"

TrackControls::TrackControls() {
    // Constructor
    volume = 1.0f; // Normal volume
    is_muted = false;
    is_mono_track = false; // Not a mono track
}

TrackControls::~TrackControls() {
    // Destructor
}

void TrackControls::set_volume(int track_id, float volume) {
    // More logic needed after this point
    this->volume = volume;
    std::cout << "Track " << track_id << " volume set to: " << volume << std::endl;
}

void TrackControls::mute_track(int track_id) {
    is_muted = true;
    std::cout << "Track " << track_id << " muted." << std::endl;
}

void TrackControls::mono_track(int track_id) {
    is_mono_track = true;
    std::cout << "Track " << track_id << " set as mono." << std::endl;
}
