#ifndef TRACK_H
#define TRACK_H

#include <iostream>

class TrackControls {
public:
    TrackControls();
    ~TrackControls();

    void set_volume(int track_id, float volume);
    void mute_track(int track_id);
    void mono_track(int track_id);

private:
    float volume;
    bool is_muted;
    bool is_mono_track;
};

#endif // TRACK_H
