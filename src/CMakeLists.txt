set(PROJECT_NAME ${CMAKE_PROJECT_NAME})

set(TICKET_SYSTEM_SOURCE_CPPS
  ticket_system.cpp
)

add_library(ticket_system ${TICKET_SYSTEM_SOURCE_CPPS})

target_link_libraries(ticket_system bustub)
target_include_directories(ticket_system PUBLIC ${BUSTUB_INCLUDE_DIR} ${TICKET_SYSTEM_INCLUDE_DIR})