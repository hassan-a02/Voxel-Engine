cmake_minimum_required(VERSION 3.10)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 17)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/glad/include
    ${CMAKE_SOURCE_DIR}/lib/glm
)

add_subdirectory(lib/glfw)

add_executable(VoxelEngine
    src/main.cpp
    src/Application.cpp
    src/Renderer.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Block.cpp
    src/Chunk.cpp
    src/stb_image.cpp
    lib/glad/src/glad.c
)

if (WIN32)
    target_link_libraries(VoxelEngine glfw opengl32)
elseif(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    target_link_libraries(VoxelEngine glfw ${OpenGL_LIBRARY})
else()
    target_link_libraries(VoxelEngine glfw GL)
endif()

add_custom_command(TARGET VoxelEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/textures $<TARGET_FILE_DIR:VoxelEngine>/textures)

add_custom_command(TARGET VoxelEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:VoxelEngine>/shaders)