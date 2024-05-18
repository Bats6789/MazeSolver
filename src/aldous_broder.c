#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MazeTools.h"
#include "aldous_broder.h"

void aldousBroder(Maze_t *maze) {
	Point_t point;
	size_t sz = maze->width * maze->height;
	size_t cellsNotVisitedCount = sz;
	size_t index;

	srand(time(NULL));

	point.x = rand() % maze->width;
	point.y = rand() % maze->height;

	index = pointToIndex(point, maze->width);

	maze->cells[index].visited = 1;
	cellsNotVisitedCount--;

	while (cellsNotVisitedCount > 0) {
		Direction_t dir = getRandomDirection(point, *maze);
		Point_t newPoint = pointShift(point, dir);
		size_t newIndex = pointToIndex(newPoint, maze->width);

		if (!maze->cells[newIndex].visited) {
			mazeConnectCells(maze, index, newIndex, dir);
			maze->cells[newIndex].visited = 1;
			cellsNotVisitedCount--;
		}

		index = newIndex;
		point = newPoint;
	}

	for (size_t i = 0; i < sz; i++) {
		maze->cells[i].visited = 0;
	}

    // assign start and stop location
	assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void aldousBroderWithSteps(Maze_t *maze, FILE *restrict stream) {
	Point_t point;
	size_t sz = maze->width * maze->height;
	size_t cellsNotVisitedCount = sz;
	size_t index;

	srand(time(NULL));

	point.x = rand() % maze->width;
	point.y = rand() % maze->height;

	index = pointToIndex(point, maze->width);

	maze->cells[index].visited = 1;
	maze->cells[index].observing = 1;
	fprintStepIgnoreVisted(stream, maze);
	cellsNotVisitedCount--;

	while (cellsNotVisitedCount > 0) {
		Direction_t dir = getRandomDirection(point, *maze);
		Point_t newPoint = pointShift(point, dir);
		size_t newIndex = pointToIndex(newPoint, maze->width);

		if (!maze->cells[newIndex].visited) {
			mazeConnectCells(maze, index, newIndex, dir);
			maze->cells[newIndex].visited = 1;
			cellsNotVisitedCount--;
		}
		maze->cells[index].observing = 0;
		maze->cells[newIndex].observing = 1;
		fprintStepIgnoreVisted(stream, maze);

		index = newIndex;
		point = newPoint;
	}
	maze->cells[index].observing = 0;

	for (size_t i = 0; i < sz; i++) {
		maze->cells[i].visited = 0;
	}

    // assign start and stop location
	assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
