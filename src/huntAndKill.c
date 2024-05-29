#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "huntAndKill.h"
#include "MazeTools.h"

static Point_t hunt(Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    size_t dirSz, newI;
    Point_t point, newPoint;
    Direction_t dir[4];

    for (size_t i = 0; i < sz; i++) {
        if (maze->cells[i].visited == 0) {
            point = indexToPoint(i, maze->width);
            dirSz = getRandomDirections(point, *maze, dir);

            for (size_t j = 0; j < dirSz; j++) {
                newPoint = pointShift(point, dir[j]);
                newI = pointToIndex(newPoint, maze->width);

                if (maze->cells[newI].visited == 1) {
                    mazeConnectCells(maze, i, newI, dir[j]);
                    return indexToPoint(i, maze->width);
                }
            }
        }
    }

    return (Point_t){0, 0};
}

static Point_t huntWithSteps(Maze_t *maze, FILE *restrict stream) {
    Point_t point = {0, 0};
    Point_t foundPoint = {0, 0};
    Point_t newPoint;
    size_t i = 0;
    size_t dirSz, newI;
    bool found = false;
    Direction_t dir[4];
    Direction_t foundDir;

    for (point.y = 0; point.y < maze->height && !found; ++point.y) {
        for (point.x = 0; point.x < maze->width; ++point.x) {
			i = pointToIndex(point, maze->width);
            maze->cells[i].observing = 1;
            if (maze->cells[i].visited == 0 && !found) {
                dirSz = getRandomDirections(point, *maze, dir);

                for (size_t j = 0; j < dirSz; j++) {
                    newPoint = pointShift(point, dir[j]);
                    newI = pointToIndex(newPoint, maze->width);

                    if (maze->cells[newI].visited == 1) {
                        found = true;
                        foundPoint = point;
                        foundDir = dir[j];
                        break;
                    }
                }
            }
        }

        fprintStepIgnoreVisted(stream, maze);

        for (point.x = 0; point.x < maze->width; ++point.x) {
			i = pointToIndex(point, maze->width);
            maze->cells[i].observing = 0;
        }
    }


    if (found) {
		i = pointToIndex(foundPoint, maze->width);
		maze->cells[i].observing = 1;
		fprintStepIgnoreVisted(stream, maze);
		maze->cells[i].observing = 0;

        mazeConnectCells(maze, i, newI, foundDir);
        fprintStepIgnoreVisted(stream, maze);
    }

    return foundPoint;
}

void huntAndKillGen(Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    size_t dirSz, newI;
    bool found = false;
    bool wallBroken = false;
    Point_t point, newPoint;
    Direction_t dir[4];

    srand(time(NULL));

    point.x = rand() % maze->width;
    point.y = rand() % maze->height;

	maze->cells[pointToIndex(point, maze->width)].visited = 1;

    do {
        do {
            dirSz = getRandomDirections(point, *maze, dir);

            wallBroken = false;

            for (size_t i = 0; i < dirSz && !wallBroken; i++) {
                newPoint = pointShift(point, dir[i]);
                newI = pointToIndex(newPoint, maze->width);

                if (maze->cells[newI].visited == 0) {
					mazeBreakWall(maze, point, dir[i]);
                    maze->cells[newI].visited = 1;
                    wallBroken = true;
                    point = newPoint;
                }
            }
        } while (wallBroken);

        point = hunt(maze);
        found = maze->cells[pointToIndex(point, maze->width)].visited == 1;
        maze->cells[pointToIndex(point, maze->width)].visited = 1;
    } while (!found);

    for (size_t i = 0; i < sz; i++) {
        maze->cells[i].visited = 0;
    }

    // assign start and stop location
    assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void huntAndKillGenWithSteps(Maze_t *maze, FILE *restrict stream) {
    size_t sz = maze->width * maze->height;
    size_t dirSz, newI;
    bool found = false;
    bool wallBroken = false;
    Point_t point, newPoint;
    Direction_t dir[4];

    srand(time(NULL));
	
	fprintStep(stream, maze);

    point.x = rand() % maze->width;
    point.y = rand() % maze->height;

	maze->cells[pointToIndex(point, maze->width)].visited = 1;

    do {
        do {
            dirSz = getRandomDirections(point, *maze, dir);

            wallBroken = false;

            for (size_t i = 0; i < dirSz && !wallBroken; i++) {
                newPoint = pointShift(point, dir[i]);
                newI = pointToIndex(newPoint, maze->width);

                if (maze->cells[newI].visited == 0) {
					mazeBreakWall(maze, point, dir[i]);
                    maze->cells[newI].visited = 1;
                    wallBroken = true;
                    point = newPoint;
                    fprintStepIgnoreVisted(stream, maze);
                }
            }
        } while (wallBroken);

        point = huntWithSteps(maze, stream);
        found = maze->cells[pointToIndex(point, maze->width)].visited == 1;
        maze->cells[pointToIndex(point, maze->width)].visited = 1;
    } while (!found);

    for (size_t i = 0; i < sz; i++) {
        maze->cells[i].visited = 0;
    }

    // assign start and stop location
    assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
