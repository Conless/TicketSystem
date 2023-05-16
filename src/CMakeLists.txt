set(PROJECT_NAME ${CMAKE_PROJECT_NAME})

set(USER_SOURCE_CPPS
  user/user_system.cpp
  user/user_type.cpp
)

set(TRAIN_SOURCE_CPPS
  train/train_system.cpp
  train/train_type.cpp
)

set(UTILS_SOURCE_CPPS
  utils/token_scanner.cpp
  utils/utility.cpp
)

add_library(ticket_system ${USER_SOURCE_CPPS} ${TRAIN_SOURCE_CPPS} ${UTILS_SOURCE_CPPS} ticket_system.cpp)

target_link_libraries(ticket_system bustub)
target_link_libraries(ticket_system array)