cmake_minimum_required(VERSION 3.9)

project(MazeSolver VERSION 1.0 LANGUAGES C)

set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_library(MazeViewer STATIC src/MazeViewer.c)
add_library(MazeTools STATIC src/MazeTools.c
								src/aldous_broder.c
								src/binaryTree.c
								src/eller.c
								src/growing_tree.c
								src/huntAndKill.c
								src/kruskal.c
								src/main.c
								src/prim.c
								src/recursiveBacktracking.c
								src/recursiveDivision.c
								src/sidewinder.c
								src/wilson.c
								src/depthFirst.c
								src/breadthFirst.c
								src/dijkstra.c
								src/aStar.c
							)

add_executable(MazeSolver src/main.c)

target_include_directories(MazeSolver PUBLIC include)
target_include_directories(MazeViewer PUBLIC include)
target_include_directories(MazeTools PUBLIC include)
target_link_libraries(MazeSolver MazeViewer MazeTools m)
