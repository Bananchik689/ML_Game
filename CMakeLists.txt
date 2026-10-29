cmake_minimum_required(VERSION 3.24)
project(DodgeGame LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.tar.gz
)

FetchContent_MakeAvailable(raylib)

add_executable(DodgeGame src/main.cpp src/Player.cpp src/Projectile.cpp src/Game.cpp src/Agent.cpp)
target_link_libraries(DodgeGame PRIVATE raylib)