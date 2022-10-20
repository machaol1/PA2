cmake_minimum_required(VERSION 3.21)
project(semestralniPrace_bomber)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        semestralniPrace_bomber
        src/main.cpp
        src/Map.cpp src/Map.h
        src/Buff.cpp src/Buff.h
        src/Player.cpp src/Player.h
        src/Game.cpp src/Game.h
        src/Bomb.cpp src/Bomb.h
        src/Enemy.cpp src/Enemy.h
        src/Object.cpp src/Object.h
        src/Movable.cpp src/Movable.h
        src/Wall.cpp src/Wall.h
        src/Menu.cpp src/Menu.h src/mainpage.h)
