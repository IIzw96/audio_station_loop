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
    UI();
    ~UI();

    // Define the other functions we will have to call
    void save_to_database();
    void load_from_database();

    void set_pan(int track_id, float pan);
    void set_time_stretch(float ratio);
    void slip_tracks(int track_id, int offset);

    // Depending on how in depth we want to go, we will likely want to include other
    // declarations in the future that will allow functionality more similar to an
    // audio loop station. 

private:
    DatabaseManager db_manager;
    LoopControls loop_controls;
    PlaybackProcessor playback_processor;
    RecordingProcessor recording_processor;
    TrackControls track_controls;
    
}

#endif // UI_H