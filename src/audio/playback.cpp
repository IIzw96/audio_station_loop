#include "playback.h"

Playback::Playback(int channels) : num_channels(channels), volume(1.0f) {
    // Constructor
    is_playing = false;

    try {
        audio_output = new RtAudio(RtAudio::UNSPECIFIED);
    } catch (RtAudioError& e) {
        std::cerr << "RtAudio initialization error: " << e.getMessage() << std::endl;
        throw std::runtime_error("Failed to initialize RtAudio.");
    }
}

Playback::~Playback() {
    // Destructor
    if (audio_output -> isStreamOpen()) {
        try {
            audio_output -> closeStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio closing error: " << e.getMessage() << std::endl;
        }
    }
    delete audio_output;
}

void Playback::play() {
    if (!is_playing) {
        // Open an audio stream
        RtAudio::StreamParameters params;
        params.deviceId = audio_output -> getDefaultOutputDevice();
        params.nChannels = num_channels;

        RtAudio::StreamOptions options;
        options.flags = RTAUDIO_SCHEDULE_REALTIME;

        unsigned int buffer_frames = FRAMES_PER_BUFFER;

        try {
            audio_output -> openStream(&params, nullptr, RTAUDIO_FLOAT32, SAMPLE_RATE, &buffer_frames, audio_callback, this, &options);
            audio_output -> startStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio startStream error: " << e.getMessage() << std::endl;
            if (audio_output -> isStreamOpen()) {
                audio_output -> closeStream();
            }
            throw std::runtime_error("Failed to play");
        }

        is_playing = true;
        std::cout << "Playback started." << std::endl;
    } else {
        std::cout << "Already playing." << std::endl;
    }
}

void Playback::pause() {
    if (is_playing) {
        // Attempt to pause the stream
        try {
            audio_output -> stopStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio stream pause error: " << e.getMessage() << std::endl;
            throw std::runtime_error("Failed to pause RtAudio stream.");
        }
        is_playing = false;
        std::cout << "Playback paused" << std::endl;
    } else {
        std::cout << "Nothing to pause" << std::endl;
    }
}

void Playback::resume() {
    if (!is_playing) {
        // Close the current stream if it didn't get closed during pause.
        // Realistically, this should never happen
        if (audio_output -> isStreamOpen()) {
            try {
                audio_output -> closeStream();
            } catch (RtAudioError& e) {
                std::cerr << "RtAudio stream closing error: " << e.getMessage() << std::endl;
                throw std::runtime_error("Failed to close RtAudio stream.");
            }
        }

        // Reopen the audio stream with new settings
        RtAudio::StreamParameters params;
        params.deviceId = audio_output -> getDefaultOutputDevice();
        params.nChannels = num_channels;

        RtAudio::StreamOptions options;
        options.flags = RTAUDIO_SCHEDULE_REALTIME;

        unsigned int buffer_frames = FRAMES_PER_BUFFER;

        try {
            audio_output -> openStream(&params, nullptr, RTAUDIO_FLOAT32, SAMPLE_RATE, &buffer_frames, audio_callback, this, &options);
            audio_output -> startStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio startStream error: " << e.getMessage() << std::endl;
            if (audio_output -> isStreamOpen()) {
                audio_output -> closeStream();
            }
            throw std::runtime_error("Failed to start RtAudio stream.");
        }

        is_playing = true;
        std::cout << "Playback resumed." << std::endl;
    } else {
        std::cout << "Already playing." << std::endl;
    }
}

void Playback::stop() {
    if (is_playing) {
        // Attempt to stop the stream
        try {
            audio_output -> stopStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio stream stopping error: " << e.getMessage() << std::endl;
            throw std::runtime_error("Failed to stop RtAudio stream.");
        }
        is_playing = false;
        std::cout << "Playback stopped";
    } else {
        std::cout << "Nothing to stop" << std::endl;
    }
}

void Playback::set_loop() {
    // Do loop stuff
}

void Playback::set_volume(float value) {
    // Verify volume ratio between 0 and 1
    if (value >= 0.0f && value <= 1.0f) {
        volume = value;
    } else {
        std::cerr << "Invalid valume ratio" << std::endl;
    }
}

float Playback::get_volume() const {
    return volume;
}

bool Playback::get_is_playing() const {
    return is_playing;
}

int Playback::audio_callback(void* output_buffer, void* input_buffer, unsigned int buffer_size,
                             double time_info, RtAudioStreamStatus status, void* user_data) {
    float* output = static_cast<float*>(output_buffer);
    Playback* player = static_cast<Playback*>(user_data);

    // Apply volume to the audio data
    for (unsigned int i = 0; i < buffer_size; ++i) {
        output[i] *= player -> get_volume();
    }

    return 0;
}
