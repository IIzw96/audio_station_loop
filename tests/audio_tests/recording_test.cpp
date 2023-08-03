#include "../src/audio/recording.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>


class RecordingProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a fresh instance of RecordingProcessor before each test
        recordingProcessor = new RecordingProcessor(1);
    }

    void TearDown() override {
        // Clean up after each test
        recordingProcessor -> stop();
        delete recordingProcessor;
    }

    // Helper function to simulate recording audio data
    void SimulateRecording(unsigned long num_frames, const float* audio_data) {
        recordingProcessor->store_audio_to_buffer(audio_data, num_frames);
    }
    // Member variable to hold the RecordingProcessor instance
    RecordingProcessor* recordingProcessor;
};

TEST_F(RecordingProcessorTest, DefaultConstructor) {
    // Test default constructor
    EXPECT_FALSE(recordingProcessor -> get_recording());
}

TEST_F(RecordingProcessorTest, RecordFunction) {
    // Test recording functionality
    EXPECT_NO_THROW(recordingProcessor -> record());
    EXPECT_TRUE(recordingProcessor -> get_recording());

    // Test recording when already recording
    recordingProcessor -> record();
    EXPECT_TRUE(recordingProcessor -> get_recording());
}

TEST_F(RecordingProcessorTest, PauseFunction) {
    // Test pause functionality
    recordingProcessor -> record();
    EXPECT_NO_THROW(recordingProcessor -> pause());
    EXPECT_FALSE(recordingProcessor -> get_recording());

    // Test pause when not recording
    EXPECT_NO_THROW(recordingProcessor -> pause());
}

TEST_F(RecordingProcessorTest, ResumeFunction) {
    // Test resume functionality
    recordingProcessor -> record();
    recordingProcessor -> pause();

    EXPECT_NO_THROW(recordingProcessor -> resume());
    EXPECT_TRUE(recordingProcessor -> get_recording());

    // Test resume when already recording
    EXPECT_NO_THROW(recordingProcessor -> resume());
    EXPECT_TRUE(recordingProcessor -> get_recording());
}

TEST_F(RecordingProcessorTest, StoreAudioToBuffer) {
        std::vector<float> audio_data = {0.1f, 0.2f, 0.3f, 0.4f};
        unsigned long num_frames = audio_data.size();
        recordingProcessor -> store_audio_to_buffer(audio_data.data(), num_frames);

        // Check if the audio_buffer contains the same data as audio_data
        const std::vector<float>& buffer = recordingProcessor -> get_audio_buffer();
        EXPECT_EQ(buffer, audio_data);
}

TEST_F(RecordingProcessorTest, SaveAudioToFile) {
    // Test saving audio data to file
    std::vector<float> audio_data = {0.1f, 0.2f, 0.3f, 0.4f};

    // Store the sample audio data in the buffer
    recordingProcessor -> store_audio_to_buffer(audio_data.data(), audio_data.size());

    // Save the audio data to the file
    recordingProcessor -> save_audio_to_file("recorded_audio.wav");

    // Check if the file was saved successfully
    ASSERT_TRUE(std::ifstream("recorded_audio.wav").good()) << "Failed to save the audio file.";

    // Use libsndfile to read the saved file and compare its content to the original audio_data
    SF_INFO sf_info;
    sf_info.format = 0; // Read format only
    SNDFILE* file = sf_open("recorded_audio.wav", SFM_READ, &sf_info);
    ASSERT_TRUE(file != nullptr) << "Failed to open the saved audio file for reading.";

    // Check if the file format and channels are as expected
    ASSERT_EQ(sf_info.format & SF_FORMAT_TYPEMASK, SF_FORMAT_WAV) << "Invalid file format.";
    ASSERT_EQ(sf_info.channels, 1) << "Unexpected number of channels.";

    // Read the data from the file
    std::vector<float> read_audio_data(audio_data.size());
    sf_count_t num_frames_read = sf_readf_float(file, read_audio_data.data(), audio_data.size());

    // Check if the correct number of frames were read
    ASSERT_EQ(num_frames_read, static_cast<sf_count_t>(audio_data.size())) << "Failed to read audio data from the file.";

    // Compare the data read from the file to the original audio_data
    for (size_t i = 0; i < audio_data.size(); ++i) {
        ASSERT_FLOAT_EQ(audio_data[i], read_audio_data[i]) << "Audio data mismatch at frame " << i;
    }

    // Close the file
    sf_close(file);
}
