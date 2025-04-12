# Thread-Safe Bounded Queue (SPSC) in Modern C++

---

## 1. Project Overview

This project implements a high-performance, thread-safe, bounded queue in modern C++. It is optimized for **Single Producer, Single Consumer (SPSC)** communication patterns in concurrent applications.

Built with strict memory control, minimal synchronization overhead, and robust testing, this queue is ideal for real-time or low-latency systems where performance and predictability matter.

---

## 2. Design Overview

The `Queue<T>` class is a fully templated, lock-free-style circular buffer supporting:

- One writer thread (producer) and one reader thread (consumer).
- Blocking and timeout-enabled popping.
- Overwriting of oldest data when full.

Design principles include:

- **Manual memory management** using `new[]`, no STL containers.
- **Thread-safe synchronization** using `std::atomic` and `std::condition_variable`.
- **Doxygen-style documentation** for clarity and auto-generated docs.
- **Modern CMake project layout** for portability and ease of build.

---

## 3. Key Features

- Circular buffer with wraparound logic
- Lock-free `Push()` using atomics
- Blocking and timeout-based `Pop()`
- Drops oldest element when full
- No STL containers for element storage
- Template class for generic support
- Doxygen-ready documentation
- Integrated unit tests with GoogleTest

---

## 4. Google Test Integration (Automatic)

GoogleTest is integrated using **CMake’s FetchContent** module. No manual download or installation is needed.

At configuration time, CMake will:

- Download GoogleTest from its GitHub release archive
- Build and link it automatically with your test suite
- Exclude unnecessary components (e.g., GMock)

GTest Version: `1.12.1`

---

## 5. Project Structure

```
/project-root
├── include/               # Header-only queue class
│   └── custom_queue.h
├── src/                   # Example producer/consumer demo
│   └── main.cpp
├── tests/                 # Unit tests (Google Test)
│   └── test_queue.cpp
├── CMakeLists.txt         # Build configuration
├── README.md              # You're here
```

---

## 6. Quick Start (with CMake)

### Prerequisites

- CMake ≥ 3.14
- C++17-compatible compiler:
  - GCC / Clang (Linux/macOS)
  - MSVC (Windows)
- Git

---

### Cloning from GitHub

```bash
git clone https://github.com/SayedAdil72/ThreadSafeQueue_CPP.git
cd thread-safe-queue
```

---

### Build Instructions (Windows - Visual Studio)

```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release

# Run the demo and tests
Release\queue_main.exe
ctest --verbose -C Release
```

---

### Build Instructions (Linux / macOS)

#### Dependencies (Ubuntu/Debian):
```bash
sudo apt update
sudo apt install -y build-essential cmake git
```

#### Dependencies (macOS with Homebrew):
```bash
brew install cmake
```

#### Build and Run:
```bash
mkdir build && cd build
cmake ..
make

# Run the example
./queue_main

# Run unit tests
ctest --verbose
```

---

## 7. Need Help?

This project is actively maintained and open to contributions. If you'd like to:

- Extend this for multi-producer/multi-consumer
- Package it as a reusable C++ library
- Generate Doxygen-based HTML docs
- Add CI/CD or benchmarking support

