cmake_minimum_required(VERSION 3.14)
project(beignets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(ext/raylib)

add_executable(beignets src/main.cpp)

target_include_directories(beignets PRIVATE include/)
target_link_libraries(beignets PRIVATE raylib)

set_target_properties(beignets PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

add_custom_command(TARGET beignets POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:beignets>/assets
)