cmake_minimum_required(VERSION 3.23)
project(SkinChanger C)

set(CMAKE_C_STANDARD 99)

add_executable(SkinChanger src/SkinChanger.c src/utils.c src/SkinChanger.h)