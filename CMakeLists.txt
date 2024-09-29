cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

project(BEEngine VERSION 1.0 LANGUAGES CXX C)

add_subdirectory(common/glfw-3.4)

add_subdirectory(common/glad)

add_subdirectory(common/stb_image)

add_subdirectory(BERender)

add_subdirectory(VKBERender)

add_subdirectory(Test)