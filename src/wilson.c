#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MazeTools.h"

static Point_t getRandomUnvistedPoint(Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    size_t unvistedIndexes[sz];
    size_t unvistedSz = 0;

    for (size_t i = 0; i < sz; i++) {
        if (maze->cells[i].visited == 0) {
            unvistedIndexes[unvistedSz++] = i;
        }
    }

    return indexToPoint(unvistedIndexes[rand() % unvistedSz], maze->width);
}

void wilsonGen(Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    size_t unvistedCellCount = sz;
    size_t i;
    Point_t point, startPoint;
    Direction_t travelVectors[sz];
    Direction_t dir;

    srand(time(NULL));

    point.x = rand() % maze->width;
    point.y = rand() % maze->height;

    maze->cells[pointToIndex(point, maze->width)].visited = 1;
    unvistedCellCount--;

    while (unvistedCellCount > 0) {
        startPoint = point = getRandomUnvistedPoint(maze);

        i = pointToIndex(point, maze->width);
        maze->cells[i].queued = 1;
        while (maze->cells[i].visited == 0) {
            dir = getRandomDirection(point, *maze);
            travelVectors[i] = dir;
            point = pointShift(point, dir);
            i = pointToIndex(point, maze->width);
            maze->cells[i].queued = 1;
        }

        while (startPoint.x != point.x || startPoint.y != point.y) {
            i = pointToIndex(startPoint, maze->width);
            Point_t newPoint = pointShift(startPoint, travelVectors[i]);
			mazeBreakWall(maze, startPoint, travelVectors[i]);
			startPoint = newPoint;
			maze->cells[i].queued = 0;
			maze->cells[i].visited = 1;
			unvistedCellCount--;
        }

        for (size_t i = 0; i < sz; i++) {
            maze->cells[i].queued = 0;
        }
    }

    for (size_t i = 0; i < sz; i++) {
        maze->cells[i].visited = 0;
    }

    // assign start and stop location
	assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void wilsonGenWithSteps(Maze_t *maze, FILE *restrict stream) {
    size_t sz = maze->width * maze->height;
    size_t unvistedCellCount = sz;
    size_t i;
    Point_t point, startPoint;
    Direction_t travelVectors[sz];
    Direction_t dir;

    srand(time(NULL));

	fprintStepIgnoreVisted(stream, maze);

    point.x = rand() % maze->width;
    point.y = rand() % maze->height;

    maze->cells[pointToIndex(point, maze->width)].visited = 1;
    maze->cells[pointToIndex(point, maze->width)].observing = 1;
    unvistedCellCount--;

	fprintStepIgnoreVisted(stream, maze);

    while (unvistedCellCount > 0) {
        startPoint = point = getRandomUnvistedPoint(maze);

        i = pointToIndex(point, maze->width);
        maze->cells[i].queued = 1;
		maze->cells[i].observing = 1;
		fprintStepIgnoreVisted(stream, maze);

        while (maze->cells[i].visited == 0) {
			maze->cells[i].observing = 0;

            dir = getRandomDirection(point, *maze);
            travelVectors[i] = dir;
            point = pointShift(point, dir);
            i = pointToIndex(point, maze->width);

            maze->cells[i].queued = 1;
            maze->cells[i].observing = 1;
			fprintStepIgnoreVisted(stream, maze);
        }

		maze->cells[pointToIndex(startPoint, maze->width)].observing = 1;
		fprintStepIgnoreVisted(stream, maze);

        while (startPoint.x != point.x || startPoint.y != point.y) {
            i = pointToIndex(startPoint, maze->width);
			maze->cells[i].observing = 0;

            Point_t newPoint = pointShift(startPoint, travelVectors[i]);
			size_t newI = pointToIndex(newPoint, maze->width);

			mazeBreakWall(maze, startPoint, travelVectors[i]);
			startPoint = newPoint;
			maze->cells[i].queued = 0;
			maze->cells[i].visited = 1;
			maze->cells[newI].observing = 1;
			unvistedCellCount--;
			fprintStepIgnoreVisted(stream, maze);
        }

		maze->cells[pointToIndex(point, maze->width)].observing = 0;

        for (size_t i = 0; i < sz; i++) {
            maze->cells[i].queued = 0;
        }
		fprintStepIgnoreVisted(stream, maze);
    }

    for (size_t i = 0; i < sz; i++) {
        maze->cells[i].visited = 0;
    }

    // assign start and stop location
	assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
