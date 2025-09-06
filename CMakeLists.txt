cmake_minimum_required(VERSION 3.10)
project(Void_Engine)

add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/vendor/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
)

find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(main
    ./src/main.cpp
    ./src/entities/grass.cpp
    # ./src/renderer/text/TextBlitter.cpp
)

target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/freetype/include/freetype2  
)

# add_library(Freetype STATIC IMPORTED)
# set_target_properties(Freetype PROPERTIES
#     IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/vendor/freetype/libfreetype.a"
#     INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/vendor/freetype/include/freetype2"
# )

target_link_libraries(main PRIVATE
    glad
    glfw
    assimp::assimp
    # Freetype
)

