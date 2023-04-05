cmake_minimum_required(VERSION 3.11)
project(TicketSystem)

set(CMAKE_CXX_FLAGS "-g -O2 -std=c++17")
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# Main sources
include_directories(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(src)