#include "ui.h"

UI::UI(DatabaseManager& db_manager) : db_manager(db_manager) {
    // Constructor
    loop_controls = new LoopControls();
    playback = new Playback(2); // Default to two channels
    recording_processor = new RecordingProcessor(2); // Default to two channels
    track_controls = new TrackControls();
    panning = new Panning();
    time_stretching = new TimeStretching();
    slip = new Slip();
}

UI::~UI() {
    // Destructor
    delete loop_controls;
    delete playback;
    delete recording_processor;
    delete track_controls;
    delete panning;
    delete time_stretching;
    delete slip;
}

void UI::save_to_database() {
    db_manager.save_database();
    std::cout << "Data saved to the database." << std::endl;
}

void UI::load_from_database(const char* table_name) {
    std::string data = db_manager.retrieve_data("some_table_name");
    std::cout << "Data loaded from the database: " << data << std::endl;
}

void UI::set_pan(int track_id, float pan) {
    panning -> set_pan(track_id, pan);
    std::cout << "Track " << track_id << " pan set to: " << pan << std::endl;
}

void UI::set_time_stretch(float ratio, int channels) {
    time_stretching -> set_time_stretch_ratio(ratio, channels);
    std::cout << "Time stretch ratio set to: " << ratio << std::endl;
}

void UI::slip_tracks(int track_id, int offset) {

    slip -> slip_track(track_id, offset);
    std::cout << "Slipped track " << track_id << " by " << offset << " samples." << std::endl;
}