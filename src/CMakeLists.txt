set(PROJECT_NAME ${CMAKE_PROJECT_NAME})

file(GLOB SRC_LIST **.cpp)
add_executable(${PROJECT_NAME}_run ${SRC_LIST})