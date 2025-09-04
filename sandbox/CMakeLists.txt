cmake_minimum_required(VERSION 3.10)
project(Void_Engine)

add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/../vendor/glad/src/glad.c
)

target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/../glad/include
)

# Now the actual app
add_executable(main ./perlin_noise.cpp)

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/../vendor/glad/include
    ${CMAKE_SOURCE_DIR}/../vendor/
)

target_link_libraries(main PRIVATE glad glfw)
