cmake_minimum_required(VERSION 3.7)
project(Particles)

set(CMAKE_CXX_STANDARD 11)


set(SOURCE_FILES
    cpp/main.cpp
    cpp/Particle.cpp
    cpp/Particles.cpp
    h/GameLoop.h
    h/GLProgram.h
    h/Obj.h
    h/Particle.h
    h/Particles.h
    h/SDLWindow.h)
add_executable(Particles ${SOURCE_FILES})

add_custom_command(
    TARGET Particles POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/glsl $<TARGET_FILE_DIR:Particles>/glsl)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

include_directories(
        ${OPENGL_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
)

target_link_libraries(
        Particles
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${GLEW_LIBRARIES}
        m
)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -fmax-errors=1")

