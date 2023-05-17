cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

project(TicketSystem)

set(CMAKE_CXX_FLAGS "-Ofast")

set(TICKET_SYSTEM_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)

set(BUSTUB_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party/bustub/src/include)
set(BUSTUB_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party/bustub/test/include)
set(BUSTUB_THIRD_PARTY_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/third_party/bustub/third_party
    ${PROJECT_SOURCE_DIR}/third_party/bustub/third_party/fmt/include
    ${PROJECT_SOURCE_DIR}/third_party/bustub/third_party/libpg_query/include
    ${PROJECT_SOURCE_DIR}/third_party/bustub/third_party/argparse/include
    ${PROJECT_SOURCE_DIR}/third_party/bustub/third_party/cpp_random_distributions
)
set(BUSTUB_INCLUDE_DIR ${BUSTUB_SRC_INCLUDE_DIR} ${BUSTUB_TEST_INCLUDE_DIR} ${BUSTUB_THIRD_PARTY_INCLUDE_DIR})

set(ARRAY_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/third_party/array/src/include)

include_directories(${ARRAY_INCLUDE_DIR} ${BUSTUB_INCLUDE_DIR} ${BUSTUB_TEST_INCLUDE_DIR} ${BUSTUB_THIRD_PARTY_INCLUDE_DIR} ${TICKET_SYSTEM_INCLUDE_DIR})

add_subdirectory(third_party)
add_subdirectory(src)

add_executable(code src/main.cpp)

target_link_libraries(code ticket_system)
