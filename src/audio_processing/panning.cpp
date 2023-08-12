#include "panning.h"

Panning::Panning(DatabaseManager& db_manager) : database(db_manager), selected_loop_id(-1) {
    // Constructor
    audio = new RtAudio(RtAudio::UNSPECIFIED);

    RtAudio::StreamParameters output_params;
    output_params.deviceId = audio -> getDefaultOutputDevice();
    output_params.nChannels = 2;    // Stereo output

    RtAudio::StreamOptions options;
    options.flags = RTAUDIO_SCHEDULE_REALTIME;
    options.priority = 0;

    try {
        unsigned int buffer_frames = FRAMES_PER_BUFFER;
        audio -> openStream(&output_params, nullptr, RTAUDIO_FLOAT32, SAMPLE_RATE, &buffer_frames, &Panning::rtAudioCallback, this, &options);
    } catch (RtAudioError& e) {
        std::cerr << "RtAudio initialization error: " << e.getMessage() << std::endl;
        throw std::runtime_error("Failed to initialize RtAudio.");
    }
}

Panning::~Panning() {
    // Destructor
    if (audio -> isStreamOpen()) {
        try {
            audio -> closeStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio closing error: " << e.getMessage() << std::endl;
        }
    }
    delete audio;
}

int Panning::rtAudioCallback(void* output_buffer, void* /*input_buffer*/, unsigned int num_frames, double /*stream_time*/, RtAudioStreamStatus /*status*/, void* user_data) {
    Panning* panning_audio = static_cast<Panning*>(user_data);
    Stereo* out = static_cast<Stereo*>(output_buffer);

    for (unsigned int i = 0; i < num_frames; i++) {
        double t = static_cast<double>(panning_audio -> phase) / static_cast<double>(SAMPLE_RATE);
        double pan_angle = panning_audio -> pan * 0.5 * PI;
        double left_volume = std::cos(pan_angle);
        double right_volume = std::sin(pan_angle);

        out[i].left = static_cast<float>(left_volume * panning_audio -> gain);
        out[i].right = static_cast<float>(right_volume * panning_audio -> gain);

        panning_audio -> phase++;
    }

    return 0;
}

void Panning::start() {
    if (!audio -> isStreamRunning()) {
        try {
            audio -> startStream();
            std::cout << "Audio stream started." << std::endl;
        } catch (RtAudioError& e) {
            throw std::runtime_error("Error starting RtAudio stream: " + std::string(e.getMessage()));
        }
    } else {
        std::cerr << "Audio stream is already running." << std::endl;
    }
}

void Panning::stop() {
    if (audio -> isStreamRunning()) {
        try {
            audio -> stopStream();
            std::cout << "Audio stream stopped." << std::endl;
        } catch (RtAudioError& e) {
            throw std::runtime_error("Error stopping RtAudio stream: " + std::string(e.getMessage()));
        }
    } else {
        std::cerr << "Audio stream is not running." << std::endl;
    }
}

bool Panning::set_loop(int loop_id) {
    // Used if we load a loop from the database. This will go through and set the pan
    // and gain accordingly, which will then be used in the callback function
    std::string loop_data = database.retrieve_data("Loop");

    // Parse the table if data exists
    if (!loop_data.empty()) {
        std::istringstream loopInfoStream(loop_data);
        int parsed_loop_id;
        std::string parsed_loop_name;
        std::string parsed_loop_length;
        int parsed_loop_bpm;
        loopInfoStream >> parsed_loop_id >> parsed_loop_name >> parsed_loop_length >> parsed_loop_bpm;

        if (parsed_loop_id == loop_id) {
            // we should update the database and pull from it to get these values
            // however, time is a limiting factor
            pan = 0.0;
            gain = 1.0;
        }
    }
}