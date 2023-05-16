set(PROJECT_NAME ${CMAKE_PROJECT_NAME})

set(BUSTUB_SOURCE_CPPS
    bustub/src/storage/disk/disk_manager.cpp
    bustub/src/buffer/lru_k_replacer.cpp
    bustub/src/buffer/buffer_pool_manager.cpp
    bustub/src/storage/page/page_guard.cpp
    bustub/src/storage/page/b_plus_tree_page.cpp
    bustub/src/storage/page/b_plus_tree_internal_page.cpp
    bustub/src/storage/page/b_plus_tree_leaf_page.cpp
    bustub/src/storage/index/index_iterator.cpp
    bustub/src/storage/index/b_plus_tree_nts.cpp
    bustub/src/storage/index/b_plus_tree.cpp
    bustub/src/storage/index/b_plus_tree_index_nts.cpp
    bustub/src/storage/index/b_plus_tree_index.cpp
)

add_executable(example_bustub bustub/src/main.cpp)

add_library(bustub ${BUSTUB_SOURCE_CPPS})

set(ARRAY_SOURCE_CPPS
    array/src/container/array.cpp
)

add_executable(example_array array/src/main.cpp)

add_library(array ${ARRAY_SOURCE_CPPS})

target_link_libraries(example_array array)
