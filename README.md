# flow-designer

[![CMake](https://github.com/InFlowStructure/flow-designer/actions/workflows/cmake.yml/badge.svg)](https://github.com/InFlowStructure/flow-designer/actions/workflows/cmake.yml)

A cross-platform C++20 program for designing Flows. A visual graph editor for creating and editing Flows using nodes from imported modules.

## Building

To build the project with CMake, simply run
```bash
cmake -B build
cmake --build build --parallel
```

## Dependencies

This project relies on the following open source projects:
- [spdlog](https://github.com/gabime/spdlog)
- [cxxopts](https://github.com/jarro2783/cxxopts)
