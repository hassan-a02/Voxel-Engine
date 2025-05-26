# Voxel Engine

A cross-platform voxel engine written in C++17 using OpenGL, GLFW, GLAD, GLM, and stb_image.

---

## Features

- Modern OpenGL rendering
- Cross-platform (Windows, Linux, macOS)
- Portable CMake-based build system
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
git clone --recursive https://github.com/yourusername/voxel-engine.git
```

If you already cloned without `--recursive`, run:

```sh
git submodule update --init --recursive
```

---

## Building the Project

```sh
mkdir build
cd build
cmake ..
cmake --build .
```

---

## Running

- The executable will be in `build/Debug` or `build/Release` (Windows) or `build/` (Linux/macOS).
- If you use the dynamic version of GLFW, ensure the correct `glfw3.dll` is in your PATH or next to the executable (Windows only).

---

## Project Structure

```
Voxel Engine/
├─ CMakeLists.txt
├─ src/
├─ include/
├─ lib/
│   ├─ glad/
│   ├─ glfw/   # Submodule
│   └─ glm/    # Submodule
└─ ...
```

---

## Notes

- **GLAD:** Generated for OpenGL version 3.3 Core Profile.
- **GLFW/GLM:** Built from source automatically via CMake.
- **stb_image:** Included as source/header.

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