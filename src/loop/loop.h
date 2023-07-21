#ifndef LOOP_H
#define LOOP_H

#include <iostream>

class LoopControls {
public:
    LoopControls();
    ~LoopControls();

    void play();
    void pause();
    void stop();

    void set_tempo(float tempo_ratio);
    void set_key(int key);

private:
    bool is_playing;
    float tempo_ratio;
    int key;
};

#endif // LOOP_H
