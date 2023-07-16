cmake_minimum_required(VERSION 3.14)
project(audio_station_loop)

# Set the C++ standard to compile with
set(CMAKE_CXX_STANDARD 17)

# Set common compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

# Add source files
set(SOURCES
    src/main.cpp
    src/audio/playback.cpp
    src/audio/recording.cpp
    src/audio_processing/panning.cpp
    src/audio_processing/slip.cpp
    src/audio_processing/time_stretching.cpp
    src/database/database.cpp
    src/loop/loop.cpp
    src/track/track.cpp
    src/ui/ui.cpp
)

# Add the include directory, this is for our 3rd party libraries we will use
include_directories(include)

# Find required packages
find_package(SQLite3 REQUIRED)

# Add executable
add_executable(audio-loop-station ${SOURCES})

# Link Libraries. This is where boost, rtaudio, etc will go when ready
target_link_libraries(audio-loop-station PRIVATE
    SQLite::SQLite3
    sqlite3
)

# Set the output directory
set_target_properties(audio-loop-station PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

# Setup for the unit tests
enable_testing()
add_subdirectory(tests)
