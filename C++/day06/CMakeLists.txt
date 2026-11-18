
cmake_minimum_required(VERSION 3.16)

project(AIInfraCppLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dot_product
    src/dot_product.cpp
)

target_include_directories(dot_product
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(app
    src/main.cpp
)

target_link_libraries(app
    PRIVATE dot_product
)

enable_testing()

add_executable(test_dot
    tests/test_dot.cpp
)

target_link_libraries(test_dot
    PRIVATE dot_product
)

add_test(NAME DotProductTest COMMAND test_dot)
