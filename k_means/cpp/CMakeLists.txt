cmake_minimum_required(VERSION 3.0)
project(k_means LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/inc)

set(TARGET_EXEC main.out)
add_executable(${TARGET_EXEC} ${PROJECT_SOURCE_DIR}/src/main.cpp)
