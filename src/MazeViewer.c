#include "MazeViewer.h"
#include <stdio.h>

void printMaze(Maze_t maze) {
	/*
	for ( size_t i = 0; i < maze.height; i++) {
		printf("%.*s\n", (int)maze.width, maze.str + i * maze.width);
	}
	*/
	printf("%s", maze.str);
}
