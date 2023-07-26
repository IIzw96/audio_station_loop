#include "recording.h"

RecordingProcessor::RecordingProcessor(int channels) : num_channels(channels) {
    // Constructor
    is_recording = false;
    PaError err = Pa_Initialize();

    if (err != paNoError) {
        std::cerr << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;
        throw std::runtime_error("Failed to initialize PortAudio.");
    }
}

RecordingProcessor::~RecordingProcessor() {
    // Destructor
    PaError err = Pa_Terminate();
    if (err != paNoError) {
        std::cerr << "PortAudio termination error: " << Pa_GetErrorText(err) << std::endl;
    }
}

void RecordingProcessor::record() {
    if (!is_recording) {
        // Open an audio stream for recording
        PaError err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audio_callback, this);
        if (err != paNoError) {
            std::cerr << "PortAudio stream opening error: " << Pa_GetErrorText(err) << std::endl;
            throw std::runtime_error("Failed to open PortAudio stream.");
        }

        // Start the audio stream
        err = Pa_StartStream(stream);
        if (err != paNoError) {
            std::cerr << "PortAudio stream starting error: " << Pa_GetErrorText(err) << std::endl;
            Pa_CloseStream(stream);
            throw std::runtime_error("Failed to start PortAudio stream.");
        }

        is_recording = true;
        std::cout << "Recording started." << std::endl;
    }
    else {
        std::cout << "Already recording." << std::endl;
    }
}

void RecordingProcessor::pause() {
    if (is_recording) {
        // Pause the audio stream
        PaError err = Pa_StopStream(stream);
        if (err != paNoError) {
            std::cerr << "PortAudio stream stopping error: " << Pa_GetErrorText(err) << std::endl;
            throw std::runtime_error("Failed to pause PortAudio stream.");
        }

        is_recording = false;
        std::cout << "Recording paused." << std::endl;
    }
    else {
        std::cout << "Not currently recording." << std::endl;
    }
}
 
void RecordingProcessor::resume() {
    if (!is_recording) {
        // Start the audio stream again
        PaError err = Pa_StartStream(stream);
        if (err != paNoError) {
            std::cerr << "PortAudio stream starting error: " << Pa_GetErrorText(err) << std::endl;
            Pa_CloseStream(stream);
            throw std::runtime_error("Failed to resume PortAudio stream.");
        }

        is_recording = true;
        std::cout << "Recording resumed." << std::endl;
    }
    else {
        std::cout << "Already recording." << std::endl;
    }
}

void RecordingProcessor::stop() {
    if (is_recording) {
        // Stop and close the audio stream
        PaError err = Pa_CloseStream(stream);
        if (err != paNoError) {
            std::cerr << "PortAudio stream closing error: " << Pa_GetErrorText(err) << std::endl;
            throw std::runtime_error("Failed to stop and close PortAudio stream.");
        }

        is_recording = false;
        std::cout << "Recording stopped." << std::endl;

        // Save audio to file and update database
        save_audio_to_file("recorded_audio.wav");

    }
    else {
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
    sf_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; // WAV file format, 16-bit PCM

    // Open the file for writing
    SNDFILE* file = sf_open(filename.c_str(), SFM_WRITE, &sf_info);
    if (!file) {
        std::cerr << "Failed to open WAV file for writing: " << sf_strerror(file) << std::endl;
        return;
    }

    // Write the recorded audio data to the file
    sf_count_t numFramesWritten = sf_write_float(file, audio_buffer.data(), audio_buffer.size());
    if (numFramesWritten != static_cast<sf_count_t>(audio_buffer.size())) {
        std::cerr << "Failed to write audio data to WAV file." << std::endl;
    }

    sf_close(file);

    std::cout << "Recorded audio saved to: " << filename << std::endl;
}

[[maybe_unused]] int RecordingProcessor::audio_callback(const void* input_buffer, void* output_buffer [[maybe_unused]],
                                    unsigned long frames_per_buffer,
                                    const PaStreamCallbackTimeInfo* time_info [[maybe_unused]],
                                    PaStreamCallbackFlags status_flags [[maybe_unused]],
                                    void* user_data) {
    // output_buffer is not currently used but it will be beneficial when we implement the playback
    // we might consider changing this file/function name when implementing playback. It will be
    // easier to do playback within the same file as recording. Future us problem. 

    /*
    PortAudio needs a callback and will be called within PortAudio. We will use this to store the recording to a
    buffer, which we can write out to a file later, in which we can update the database appropriately.
    */
    float* input = static_cast<float*>(const_cast<void*>(input_buffer));
    RecordingProcessor* recorder = static_cast<RecordingProcessor*>(user_data);

    if (recorder -> get_recording()) {
        recorder -> store_audio_to_buffer(input, frames_per_buffer);
    }

    return paContinue;
}