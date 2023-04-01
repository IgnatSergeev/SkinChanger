cmake_minimum_required(VERSION 3.23)
project(SkinChanger C)

set(CMAKE_C_STANDARD 99)

add_executable(SkinChanger main.c src/SkinChanger.c src/utils.cpp src/utils.c)
