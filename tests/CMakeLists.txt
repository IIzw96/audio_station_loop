cmake_minimum_required(VERSION 3.14)
project(audio-loop-station-tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(SOURCES
    ../src/database/database.cpp
    ../src/audio/recording.cpp
    ../src/audio_processing/time_stretching.cpp
)

# Add test source files
set(TEST_SOURCES
    audio_tests/playback_test.cpp
    audio_tests/recording_test.cpp
    audio_processing_tests/panning_test.cpp
    audio_processing_tests/slip_test.cpp
    audio_processing_tests/time_stretching_test.cpp
    database_tests/database_test.cpp
    loop_tests/loop_test.cpp
    track_tests/track_test.cpp
    ui_tests/ui_test.cpp
)

# Include necessary directories for tests
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/tests
)

# Find required packages
find_package(SQLite3 REQUIRED)
find_package(GTest REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(FFTW REQUIRED fftw3 IMPORTED_TARGET)
pkg_search_module(SoundTouch REQUIRED soundtouch IMPORTED_TARGET)
pkg_search_module(SndFile REQUIRED IMPORTED_TARGET sndfile)
pkg_search_module(RtAudio REQUIRED IMPORTED_TARGET rtaudio)

# Add test executable
add_executable(audio-loop-station-tests ${TEST_SOURCES} ${SOURCES})

# Link necessary libraries
target_link_libraries(audio-loop-station-tests
    SQLite::SQLite3
    sqlite3
    GTest::GTest
    GTest::Main
    PkgConfig::FFTW
    PkgConfig::SoundTouch
    PkgConfig::SndFile
    PkgConfig::RtAudio
)

# Add test case(s)
add_test(NAME audio-loop-station-tests COMMAND audio-loop-station-tests)