cmake_minimum_required(VERSION 3.9)

project(MazeSolver VERSION 1.0 LANGUAGES C)

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_library(MazeViewer STATIC src/MazeViewer.c)
add_library(MazeTools STATIC src/MazeTools.c)

add_executable(MazeSolver src/main.c)

target_include_directories(MazeSolver PUBLIC include)
target_link_libraries(MazeSolver MazeViewer MazeTools)
