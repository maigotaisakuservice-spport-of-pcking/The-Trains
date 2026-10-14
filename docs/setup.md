# The Stations - Developer Setup Guide

Welcome to **The Stations** codebase! This document outlines developer setup procedures for building and contributing to the project.

## Prerequisites

- **C++ Compiler**: C++20 compliant compiler (MSVC 2022 / GCC 11+ / Clang 13+)
- **CMake**: Version 3.20 or later
- **Git**

## Platform Setup & Compilation

### Windows (MSVC)
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
ctest -C Release --output-on-failure
```

### macOS (Clang / Metal)
```bash
mkdir build
cd build
cmake ..
make -j4
ctest --output-on-failure
```

### Linux / Ubuntu (GCC / Vulkan)
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake
mkdir build
cd build
cmake ..
make -j4
ctest --output-on-failure
```

### Android (NDK)
Building for Android requires the Android NDK and CMake toolchain file:
```bash
cmake -H. -Bbuild/android \
  -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake \
  -DANDROID_ABI=arm64-v8a \
  -DANDROID_PLATFORM=android-26
cmake --build build/android
```

## Running Unit Tests

Unit tests are managed via CMake CTest. Run all unit tests with:
```bash
cd build
ctest --output-on-failure
```
