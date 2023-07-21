#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <iostream>

class Playback {
public:
    Playback();
    ~Playback();

    void play();
    void pause();
    void stop();

private:
    bool is_playing;
    float volume;
};

#endif // PLAYBACK_H