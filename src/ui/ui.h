#ifndef UI_H
#define UI_H

#include <iostream>

#include "../audio/playback.h"
#include "../audio/recording.h"
#include "../audio_processing/panning.h"
#include "../audio_processing/slip.h"
#include "../audio_processing/time_stretching.h"
#include "../database/database.h"
#include "../loop/loop.h"
#include "../track/track.h"

class UI {
public:
    // Constructors & Deconstructors
    UI(DatabaseManager& db_manager);
    ~UI();

    // Define the other functions we will have to call
    void save_to_database();
    void load_from_database(const char* table_name);

    void set_pan(int track_id, float pan);
    void set_time_stretch(float ratio);
    void slip_tracks(int track_id, int offset);
    void save_loop(int loop_num);
    void load_loop(int loop_num);
    void arm_track_1();
    void arm_track_2();
    void arm_track_3();
    void arm_track_4();
    // Depending on how in depth we want to go, we will likely want to include other
    // declarations in the future that will allow functionality more similar to an
    // audio loop station. 

private:
    DatabaseManager& db_manager;
    LoopControls* loop_controls;
    Playback* playback;
    RecordingProcessor* recording_processor;
    TrackControls* track_controls;
    Panning* panning;
    TimeStretching* time_stretching;
    Slip* slip;
    bool Track1 = false;
    bool Track2 = false;
    bool Track3 = false;
    bool Track4 = false;
};

#endif // UI_H
