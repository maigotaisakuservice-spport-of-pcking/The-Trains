cmake_minimum_required(VERSION 3.20)
project(TheStations VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform detection
if(WIN32)
    set(PLATFORM_NAME "windows")
elseif(APPLE)
    set(PLATFORM_NAME "mac")
elseif(ANDROID)
    set(PLATFORM_NAME "android")
else()
    set(PLATFORM_NAME "linux")
endif()

message(STATUS "Configuring ${PROJECT_NAME} for platform: ${PLATFORM_NAME}")

# Enable Testing
enable_testing()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shared
)

# Collect shared source files
file(GLOB_RECURSE SHARED_SOURCES
    "src/shared/*.cpp"
    "src/shared/*.hpp"
    "src/shared/*.h"
)

# Platform specific sources
file(GLOB WINDOWS_SOURCES "src/windows/*.cpp")
file(GLOB MAC_SOURCES "src/mac/*.cpp")
file(GLOB ANDROID_SOURCES "src/android/*.cpp")

if(WIN32)
    set(PLATFORM_SOURCES ${WINDOWS_SOURCES})
elseif(APPLE)
    set(PLATFORM_SOURCES ${MAC_SOURCES})
elseif(ANDROID)
    set(PLATFORM_SOURCES ${ANDROID_SOURCES})
else()
    # On Linux CI runner, compile all platform sources to verify cross-platform code compilation
    set(PLATFORM_SOURCES ${WINDOWS_SOURCES} ${MAC_SOURCES} ${ANDROID_SOURCES})
endif()

# Shared Core Library
add_library(the_stations_core ${SHARED_SOURCES} ${PLATFORM_SOURCES})

# Executable target
add_executable(TheStationsMain src/main.cpp)
target_link_libraries(TheStationsMain PRIVATE the_stations_core)

# Tests directory
add_subdirectory(tests)
