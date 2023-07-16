cmake_minimum_required(VERSION 3.14)
project(audio-loop-station-tests)

set(SOURCES
    ../src/database/database.cpp
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

# Add test executable
add_executable(audio-loop-station-tests ${TEST_SOURCES} ${SOURCES})

# Link necessary libraries
target_link_libraries(audio-loop-station-tests
    SQLite::SQLite3
    sqlite3
    GTest::GTest
    GTest::Main
)

# Add test case(s)
add_test(NAME audio-loop-station-tests COMMAND audio-loop-station-tests)
