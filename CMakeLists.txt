cmake_minimum_required(VERSION 3.19)
project(bigint)

set(CMAKE_CXX_STANDARD 14)

add_executable(bigint main.cpp Bigint.cpp Bigint.h)