# Voxel Engine

A basic voxel engine using OpenGL. (DOES NOT RUN CORRECTLY ON LINUX, NOT TESTED ON MACOS)

---
## Engine Features

- Camera movement
- Block face culling
- Static 20 x 20 chunks world (procedural world generation will be added later)

---

## Technical Features

- Basic OpenGL rendering
- Portable CMake build system
- All dependencies included as submodules or source

---

## Prerequisites

- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/) (version 3.10 or higher)
- A C++17-compatible compiler (e.g. Visual Studio 2019/2022, GCC 8+, Clang 7+)

---

## Cloning the Repository

This project uses git submodules for dependencies (GLFW, GLM).  
**Clone with submodules:**

```sh
git clone --recursive https://github.com/hassan-a02/voxel-engine.git
```

If you already cloned without `--recursive`, run:

```sh
git submodule update --init --recursive
```

---

## Building the Project

Inside the project directory in CL:

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

---

## Running

- The executable will be in `build/Debug` or `build/Release` (Windows) or `build/` (Linux/macOS).

---

## Project Structure

```
Voxel Engine/
├─ README.md
├─ CMakeLists.txt
├─ src/
├─ include/
├─ textures/
├─ shaders/
├─ lib/
│   ├─ glad/
│   ├─ glfw/   # Submodule
│   └─ glm/
```

---

## Notes

- **GLAD:** Generated for OpenGL version 3.3 Core Profile.
- **GLFW:** Built from source automatically via CMake.
- **GLM / stb_image:** Included as source/header.

---

## Troubleshooting

- If you get errors about missing submodules, run:
  ```sh
  git submodule update --init --recursive
  ```
- If you get OpenGL linking errors on Linux, ensure you have OpenGL development libraries installed (e.g., `sudo apt install libgl1-mesa-dev`).
- For macOS, Xcode and Command Line Tools should provide OpenGL.

---

## Credits

- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)
- [GLAD](https://glad.dav1d.de/)
- [stb_image](https://github.com/nothings/stb)