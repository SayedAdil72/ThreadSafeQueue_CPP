cmake_minimum_required(VERSION 3.14)
project(ThreadSafeQueue LANGUAGES CXX)

# ------------------------
# Compiler Configuration
# ------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use position-independent code (for Linux/macOS shared libs)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ------------------------
# MSVC Runtime Fix (Windows only)
# ------------------------
if(MSVC)
  message(STATUS "Forcing MSVC to use dynamic runtime (/MD, /MDd)")
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # Required for GoogleTest
  foreach(flag
      CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    if(${flag} MATCHES "/MT")
      string(REPLACE "/MT" "/MD" ${flag} "${${flag}}")
    endif()
  endforeach()
endif()

# ------------------------
# Include paths
# ------------------------
include_directories(include)

# ------------------------
# Main executable
# ------------------------
add_executable(queue_main
  src/main.cpp
)

# ------------------------
# GoogleTest Setup
# ------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(INSTALL_GTEST OFF)
set(BUILD_GMOCK OFF)
FetchContent_MakeAvailable(googletest)

# ------------------------
# Enable testing and register tests
# ------------------------
include(CTest)
enable_testing()

add_executable(queue_tests
  tests/test_queue.cpp
)

target_link_libraries(queue_tests
  gtest
  gtest_main
)

add_test(NAME QueueTests COMMAND queue_tests)
