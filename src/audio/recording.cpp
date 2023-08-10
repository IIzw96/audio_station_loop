#include "recording.h"

RecordingProcessor::RecordingProcessor(int channels) : num_channels(channels) {
    // Constructor
    is_recording = false;
    try {
        audio_input = new RtAudio(RtAudio::UNSPECIFIED);
    } catch (RtAudioError& e) {
        std::cerr << "RtAudio initialization error: " << e.getMessage() << std::endl;
        throw std::runtime_error("Failed to initialize RtAudio.");
    }
}

RecordingProcessor::~RecordingProcessor() {
    // Destructor
    if (audio_input -> isStreamOpen()) {
        try {
            audio_input -> closeStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio closing error: " << e.getMessage() << std::endl;
        }
    }
    delete audio_input;
}

void RecordingProcessor::record() {
    if (!is_recording) {
        // Open an audio stream for recording
        RtAudio::StreamParameters params;
        params.deviceId = audio_input -> getDefaultInputDevice();
        params.nChannels = num_channels;
        
        RtAudio::StreamOptions options;
        options.flags = RTAUDIO_SCHEDULE_REALTIME;

        unsigned int buffer_frames = FRAMES_PER_BUFFER;
        std::cout << params.deviceId << std::endl;
        std::cout << params.nChannels << std::endl;
        try {
            std::cout << "From here" << std::endl;
            audio_input -> openStream(&params, nullptr, RTAUDIO_FLOAT32, SAMPLE_RATE, &buffer_frames, audio_callback, this, &options);
            std::cout << "From here2" << std::endl;
            audio_input -> startStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio startStream error: " << e.getMessage() << std::endl;
            if (audio_input -> isStreamOpen()) {
                audio_input -> closeStream();
            }
            throw std::runtime_error("Failed to start RtAudio stream.");
        }
        is_recording = true;
        std::cout << "Recording started." << std::endl;
    } else {
        std::cout << "Already recording." << std::endl;
    }
}

void RecordingProcessor::pause() {
    if (is_recording) {
        // Pause the audio stream
        try {
            audio_input -> stopStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio stream stopping error: " << e.getMessage() << std::endl;
            throw std::runtime_error("Failed to pause RtAudio stream.");
        }
        is_recording = false;
        std::cout << "Recording paused." << std::endl;
    } else {
        std::cout << "Not currently recording." << std::endl;
    }
}
 
void RecordingProcessor::resume() {
    if (!is_recording) {
        // Close the current stream if it's open
        if (audio_input->isStreamOpen()) {
            try {
                audio_input->closeStream();
            } catch (RtAudioError& e) {
                std::cerr << "RtAudio stream closing error: " << e.getMessage() << std::endl;
                throw std::runtime_error("Failed to close RtAudio stream.");
            }
        }

        // Reopen the audio stream with new settings
        RtAudio::StreamParameters params;
        params.deviceId = audio_input->getDefaultInputDevice();
        params.nChannels = num_channels;

        RtAudio::StreamOptions options;
        options.flags = RTAUDIO_SCHEDULE_REALTIME;

        unsigned int buffer_frames = FRAMES_PER_BUFFER;

        try {
            audio_input->openStream(&params, nullptr, RTAUDIO_FLOAT32, SAMPLE_RATE, &buffer_frames, audio_callback, this, &options);
            audio_input->startStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio startStream error: " << e.getMessage() << std::endl;
            if (audio_input->isStreamOpen()) {
                audio_input->closeStream();
            }
            throw std::runtime_error("Failed to start RtAudio stream.");
        }

        is_recording = true;
        std::cout << "Recording resumed." << std::endl;
    } else {
        std::cout << "Already recording." << std::endl;
    }
}

void RecordingProcessor::stop() {
    if (is_recording) {
        try {
            audio_input -> stopStream();
        } catch (RtAudioError& e) {
            std::cerr << "RtAudio stream stopping error: " << e.getMessage() << std::endl;
            throw std::runtime_error("Failed to stop RtAudio stream.");
        }

        is_recording = false;
        std::cout << "Recording stopped." << std::endl;

        // Save audio to file and update database
        save_audio_to_file("recorded_audio.wav");
    } else {
        std::cout << "Not currently recording." << std::endl;
    }
}

bool RecordingProcessor::get_recording() const {
    // Basically only used in the AudioPlayback, might be a better way to accomplish this
    return is_recording;
}

void RecordingProcessor::store_audio_to_buffer(const float* audio_data, unsigned long num_frames) {
    audio_buffer.insert(audio_buffer.end(), audio_data, audio_data + num_frames);
}

void RecordingProcessor::save_audio_to_file(const std::string& filename) {
    SF_INFO sf_info;
    sf_info.channels = num_channels;
    sf_info.samplerate = SAMPLE_RATE;
    sf_info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT; // Use SF_FORMAT_FLOAT for floating-point data

    // Open the file for writing
    SNDFILE* file = sf_open(filename.c_str(), SFM_WRITE, &sf_info);
    if (!file) {
        std::cerr << "Failed to open WAV file for writing: " << sf_strerror(file) << std::endl;
        return;
    }

    // Write the recorded audio data to the file
    sf_count_t numFramesWritten = sf_writef_float(file, audio_buffer.data(), audio_buffer.size());
    if (numFramesWritten != static_cast<sf_count_t>(audio_buffer.size())) {
        std::cerr << "Failed to write audio data to WAV file." << std::endl;
    }

    sf_close(file);

    std::cout << "Recorded audio saved to: " << filename << std::endl;
}

int RecordingProcessor::audio_callback(void* output_buffer, void* input_buffer, unsigned int buffer_size,
                                       double time_info, RtAudioStreamStatus status, void* user_data) {
    // output_buffer is not currently used but it will be beneficial when we implement the playback
    // we might consider changing this file/function name when implementing playback. It may be
    // easier to do playback within the same file as recording. Future us problem.

    float* input = static_cast<float*>(input_buffer);
    RecordingProcessor* recorder = static_cast<RecordingProcessor*>(user_data);

    if (recorder -> get_recording()) {
        recorder -> store_audio_to_buffer(input, buffer_size);
    }

    return 0;
}
