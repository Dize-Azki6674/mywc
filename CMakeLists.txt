cmake_minimum_required(VERSION 4.1)

project(mywc CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(mywc src/main.cpp)

target_include_directories(mywc PRIVATE include)