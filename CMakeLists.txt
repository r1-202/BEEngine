cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_COMPILER "g++")

project(BEEngine VERSION 1.0 LANGUAGES CXX)

add_subdirectory(BERender)

add_subdirectory(Test)