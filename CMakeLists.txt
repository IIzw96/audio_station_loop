cmake_minimum_required(VERSION 3.14)
project(audio_station_loop VERSION 0.1)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

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
    mainwindow.cpp
    mainwindow.ui
)

# Add the include directory, this is for our 3rd party libraries we will use
include_directories(include)

# Find required packages
find_package(SQLite3 REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Add executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(audio_station_loop
        MANUAL_FINALIZATION
        ${SOURCES}
    )
else()
    add_executable(audio_station_loop
        ${SOURCES}
    )
endif()
# add_executable(audio-loop-station ${SOURCES})

# Link Libraries. This is where boost, rtaudio, etc will go when ready
target_link_libraries(audio_station_loop PRIVATE
    SQLite::SQLite3
    sqlite3
    Qt${QT_VERSION_MAJOR}::Widgets
)

# Set the output directory
#set_target_properties(audio_station_loop PROPERTIES
#    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

set_target_properties(audio_station_loop PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS audio_station_loop
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Setup for the unit tests
enable_testing()
add_subdirectory(tests)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(audio_station_loop)
endif()
