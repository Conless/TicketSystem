cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

project(Array)

set(CMAKE_CXX_FLAGS "-g -Ofast")

include_directories(${PROJECT_SOURCE_DIR}/src/include)

add_executable(code src/main.cpp src/array.cpp)
