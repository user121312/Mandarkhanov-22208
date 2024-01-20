cmake_minimum_required(VERSION 3.23)
project(Counter VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Counter src/main.cpp src/word_count.h src/word_count.cpp)
