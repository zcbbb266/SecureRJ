# Boost.Asio 3-Party Demo (C++20 / OpenMP) with ABY3 core

A minimal three-party demo using **C++20**, **Boost.Asio (Boost 1.71.0)**, **OpenMP**, and **ABY3 core** for secure computation primitives. A companion **`main.py`** script lets you pick test items.

---

## Requirements

- **C++20** toolchain  
  - Linux: `gcc ≥ 10` or `clang ≥ 14`  
  - Windows: MSVC 19.29+ (VS 2019 16.11+)  
  - macOS: Xcode 14+ (install `libomp`)
- **CMake ≥ 3.15**
- **Boost 1.71.0**
- **OpenMP**
- **ABY3 core** (as submodule or preinstalled library)


---


## Build

```bash
mkdir build
cd build
cmake ..
make
```
---
## Quickstart
./boost_asio_example 3 0 5 2 0 1 1&
./boost_asio_example 3 1 5 2 0 1 1&
./boost_asio_example 3 2 5 2 0 1 1&
