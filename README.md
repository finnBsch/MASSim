# Multi-Agent Simulation 
This repository contains the code for a project in the [WASP AI/ML course](https://wasp-sweden.org/graduate-school/curriculum/courses/).
The interactive demo can be found on [https://www.finnbusch.com/MASSim/](https://www.finnbusch.com/MASSim/).
The simulation is implemented as a C++ library and wrapped for WebAssembly using Emscripten.
## Build instructions
### C++ Library
The C++ library can be built independently to be used in other projects.
For this, make sure **Eigen3** is installed, install on debian-based distros ```sudo apt install libeigen3-dev```, and then build as configured in the CMakeLists.txt.
### Web Interface
When building the WebAssembly interface, make sure Emscripten is installed. Furthermore, after cloning the repo, create a directory named ```ext_include``` in the repo's root.
Clone the eigen3 source into that folder and rename the folder to eigen3. This is required since emscripten will not use system libraries. Build using ```emcc -lembind -O3 -s ALLOW_MEMORY_GROWTH -o mas_sim.js src/mas_sim/* src/bindings.cpp -I include/ -I ext_include/
``` in the root directory. The web version can then be ran locally by navigating into the repo's directory and invoking ```python3 -m http.server```.
