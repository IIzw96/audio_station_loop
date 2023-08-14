#include "ui.h"

UI::UI(DatabaseManager& db_manager) : db_manager(db_manager) {
    // Constructor
    loop_controls = new LoopControls();
    playback = new Playback(2); // Default to two channels
    recording_processor = new RecordingProcessor(2); // Default to two channels
    track_controls = new TrackControls();
    panning = new Panning(db_manager);
    time_stretching = new TimeStretching(); // Update this if we want to use something other than the defaults
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

void UI::save_loop(int loop_num) {
    std::cout << "Loop loaded or created" << std::endl;
    db_manager.create_loop(std::to_string(loop_num));
    int id = db_manager.find_loop_id(std::to_string(loop_num));

    if (Track1) {
        int status = db_manager.replace_track_to_loop(id, "Track1" + std::to_string(id));
        if (status == -1) {
            std::cerr << "Could not create loop" << std::endl;
            return;
        }
        std::cout << "Track1 saved" << std::endl;
    }
    if (Track2) {
        int status = db_manager.replace_track_to_loop(id, "Track2" + std::to_string(id));
        if (status == -1) {
            std::cerr << "Could not create loop" << std::endl;
            return;
        }
        std::cout << "Track2 saved" << std::endl;

    }
    if (Track3) {
        int status = db_manager.replace_track_to_loop(id, "Track3" + std::to_string(id));
        if (status == -1) {
            std::cerr << "Could not create loop" << std::endl;
            return;
        }
        std::cout << "Track3 saved" << std::endl;

    }
    if (Track4) {
        int status = db_manager.replace_track_to_loop(id, "Track4" + std::to_string(id));
        if (status == -1) {
            std::cerr << "Could not create loop" << std::endl;
            return;
        }
        std::cout << "Track4 saved" << std::endl;

    }
}

void UI::load_loop(int loop_num) {
    std::cout << "loading loop" << std::endl;
    int id = db_manager.find_loop_id(std::to_string(loop_num));
    std::vector<std::string> track_paths = db_manager.load_tracks_by_loop(id);

    if (track_paths[0] == "") std::cout << "No tracks with path for that loop_id" << std::endl;
    for ( unsigned long i=0; i<track_paths.size(); i++)
        std::cout << track_paths[i] << std::endl;

}

void UI::start_recording () {
    recording_processor->record();
}

void UI::stop_recording() {
    recording_processor->stop();
}

void UI::arm_track_1() {
    Track1 = !Track1;
    std::cout << Track1 << std::endl;
}
void UI::arm_track_2() {
    Track2 = !Track2;
     std::cout << Track2 << std::endl;
}
void UI::arm_track_3() {
    Track3 = !Track3;
     std::cout << Track3 << std::endl;
}
void UI::arm_track_4() {
    Track4 = !Track4;
     std::cout << Track4 << std::endl;
}

bool UI::get_track_1(){
    return Track1;
}

bool UI::get_track_2(){
    return Track2;
}

bool UI::get_track_3(){
    return Track3;
}

bool UI::get_track_4(){
    return Track4;
}
