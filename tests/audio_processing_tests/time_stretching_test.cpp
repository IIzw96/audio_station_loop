#include "../src/audio_processing/time_stretching.h"
#include <gtest/gtest.h>

class TimeStretchingTest : public ::testing::Test {
protected:
    void SetUp() override {
        ts = new TimeStretching(1.0, 1, 4000.0);
    }

    void TearDown() override {
        delete ts;
    }

    TimeStretching* ts;
};

TEST_F(TimeStretchingTest, HannWindowTest) {
    std::vector<float> input = {1.0, 2.0, 3.0, 4.0};
    std::vector<float> expected_output = { 0, 1.5, 2.25, 0 };

    ts->apply_hann_window(input);

    for (size_t i = 0; i < input.size(); ++i) {
        ASSERT_NEAR(input[i], expected_output[i], 1e-5);
    }
}

TEST_F(TimeStretchingTest, LowPassFilterTest) {
    // Create complex input data for the low-pass filter
    std::vector<std::complex<float>> input_complex = {
        {1.0, 0.0}, 
        {2.0, 0.0}, 
        {3.0, 0.0}, 
        {4.0, 0.0}
    };

    std::vector<std::complex<float>> expected_output =  {
        {0.709714, 0.0},
        {0.243779, 0.0},
        {0.307875, 0.0},
        {0.29831, 0.0}
    };
    ts->apply_low_pass_filter(input_complex, 2000.0); // Use a cutoff frequency of 2000 Hz

    for (size_t i = 0; i < input_complex.size(); ++i) {
        ASSERT_NEAR(input_complex[i].real(), expected_output[i].real(), 0.001);
        ASSERT_EQ(input_complex[i].imag(), expected_output[i].imag());
    }
}

TEST_F(TimeStretchingTest, OverlapAddTest) {
    std::vector<float> input = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    std::vector<float> expected_output = {1.0, 4.0, 6.0, 8.0, 5.0};

    std::vector<float> output;
    TimeStretching ts;
    ts.overlap_add(input, output);

    ASSERT_EQ(output.size(), expected_output.size());
    for (size_t i = 0; i < output.size(); ++i) {
        ASSERT_FLOAT_EQ(output[i], expected_output[i]);
    }
}
