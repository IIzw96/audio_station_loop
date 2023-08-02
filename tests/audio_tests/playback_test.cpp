#include "../src/constants.h"
#include "../src/audio/playback.h"
#include <gtest/gtest.h>

class PlaybackTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a fresh instance of Playback for each test
        playback = new Playback(1);
    }

    void TearDown() {
        // Clean up after each test
        playback -> stop();
        delete playback;
    }

    Playback* playback;
};

TEST_F(PlaybackTest, DefaultConstructor) {
    // Tests the default constructor
    EXPECT_EQ(playback -> get_volume(), 1.0f);
    EXPECT_FALSE(playback -> get_is_playing());
}

TEST_F(PlaybackTest, PlayFunction) {
    // Test the functionality of play
    EXPECT_FALSE(playback -> get_is_playing());
    playback -> play();
    EXPECT_TRUE(playback -> get_is_playing());

    // Try playing again, should print "Already playing."
    testing::internal::CaptureStdout();
    playback -> play();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(playback -> get_is_playing());
    EXPECT_EQ(output, "Already playing.\n");
}

TEST_F(PlaybackTest, PauseFunction) {
    // Test the functionality of pause
    EXPECT_FALSE(playback -> get_is_playing());
    playback -> play();
    EXPECT_TRUE(playback -> get_is_playing());
    
    playback->pause();
    EXPECT_FALSE(playback -> get_is_playing()); 

    // SHould print "Nothing to pause."
    testing::internal::CaptureStdout();
    playback -> pause();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_FALSE(playback -> get_is_playing());
    EXPECT_EQ(output, "Nothing to pause\n");
}

TEST_F(PlaybackTest, ResumeFunction) {
    // Test that we are able to resume playback
    EXPECT_FALSE(playback -> get_is_playing());

    // Start playing and then pause/resume
    playback -> play();
    playback -> pause();
    playback -> resume();

    EXPECT_TRUE(playback -> get_is_playing());
}

TEST_F(PlaybackTest, StopFunction) {
    // Test that the stream stops
    EXPECT_FALSE(playback -> get_is_playing());

    // Start playing and then pause/resume
    playback -> play();
    playback -> stop();

    EXPECT_FALSE(playback -> get_is_playing());
}