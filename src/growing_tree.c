#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "growing_tree.h"

static void delete(size_t *sz, size_t array[*sz], size_t pos) {
    if (pos < *sz) {
        for (size_t i = pos; i < *sz; i++) {
            array[i] = array[i + 1];
        }

        *sz -= 1;
    }
}

static size_t grabIndexFromMethod(growingTreeMethods_t method, double split,
                                  size_t sz) {
    switch (method) {
        case newestTree:
            return sz - 1;
            break;

        case middleTree:
            return sz / 2;
            break;

        case oldestTree:
            return 0;
            break;

        case randomTree:
            return rand() % sz;
            break;

        case newest_middleTree:
            if ((double)rand() / RAND_MAX < split) {
                return sz - 1;
            } else {
                return sz / 2;
            }
            break;

        case newest_oldestTree:
            if ((double)rand() / RAND_MAX < split) {
                return sz - 1;
            } else {
                return 0;
            }
            break;

        case newest_randomTree:
            if ((double)rand() / RAND_MAX < split) {
                return sz - 1;
            } else {
                return rand() % sz;
            }
            break;

        case middle_oldestTree:
            if ((double)rand() / RAND_MAX < split) {
                return sz / 2;
            } else {
                return 0;
            }
            break;

        case middle_randomTree:
            if ((double)rand() / RAND_MAX < split) {
                return sz / 2;
            } else {
                return rand() % sz;
            }
			break;

        case oldest_randomTree:
            if ((double)rand() / RAND_MAX < split) {
                return 0;
            } else {
                return rand() % sz;
            }
			break;

        case INVALID_METHOD:
            return 0;
            break;

        default:
            return 0;
            break;
    }
}

void growingTreeGen(Maze_t *maze, growingTreeMethods_t method, double split) {
    size_t sz = maze->width * maze->height;
    size_t trackedCellsSz = 0;
    size_t trackedCells[sz];
    Point_t point;

    srand(time(NULL));

    // Select first cell at random
    point.x = rand() % maze->width;
    point.y = rand() % maze->height;

    trackedCells[trackedCellsSz++] = pointToIndex(point, maze->width);
    maze->cells[trackedCells[0]].visited = 1;

    while (trackedCellsSz > 0) {
        size_t trackedI = grabIndexFromMethod(method, split, trackedCellsSz);
        size_t directionsCount = 0;
        Direction_t dir[4];
        bool foundCell = false;

        point = indexToPoint(trackedCells[trackedI], maze->width);
        directionsCount = getRandomDirections(point, *maze, dir);

        for (size_t i = 0; !foundCell && i < directionsCount; i++) {
            Point_t newPoint = pointShift(point, dir[i]);
            size_t newI = pointToIndex(newPoint, maze->width);

            if (maze->cells[newI].visited == 0) {
                mazeConnectCells(maze, trackedCells[trackedI], newI, dir[i]);
				trackedCells[trackedCellsSz++] = newI;
                maze->cells[newI].visited = 1;
                foundCell = true;
            }
        }

        if (!foundCell) {
            delete (&trackedCellsSz, trackedCells, trackedI);
        }
    }

    // reset visited
    for (size_t i = 0; i < sz; i++) {
        maze->cells[i].visited = 0;
    }

    // assign start and stop location
    assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void growingTreeGenWithSteps(Maze_t *maze, growingTreeMethods_t method,
                             double split, FILE *restrict stream) {
    size_t sz = maze->width * maze->height;
    size_t trackedCellsSz = 0;
    size_t trackedCells[sz];
    Point_t point;

    srand(time(NULL));

    // Select first cell at random
    point.x = rand() % maze->width;
    point.y = rand() % maze->height;

	fprintStepIgnoreVisted(stream, maze);

    trackedCells[trackedCellsSz++] = pointToIndex(point, maze->width);
    maze->cells[trackedCells[0]].visited = 1;
    maze->cells[trackedCells[0]].observing = 1;

	fprintStepIgnoreVisted(stream, maze);

    while (trackedCellsSz > 0) {
        size_t trackedI = grabIndexFromMethod(method, split, trackedCellsSz);
        size_t directionsCount = 0;
        Direction_t dir[4];
        bool foundCell = false;

        point = indexToPoint(trackedCells[trackedI], maze->width);
        directionsCount = getRandomDirections(point, *maze, dir);

        for (size_t i = 0; !foundCell && i < directionsCount; i++) {
            Point_t newPoint = pointShift(point, dir[i]);
            size_t newI = pointToIndex(newPoint, maze->width);

            if (maze->cells[newI].visited == 0) {
                mazeConnectCells(maze, trackedCells[trackedI], newI, dir[i]);
				trackedCells[trackedCellsSz] = newI;
				maze->cells[trackedCells[trackedCellsSz++]].observing = 1;
                maze->cells[newI].visited = 1;
                foundCell = true;
            }
        }

        if (!foundCell) {
            maze->cells[trackedCells[trackedI]].observing = 0;
            delete (&trackedCellsSz, trackedCells, trackedI);
        }
		fprintStepIgnoreVisted(stream, maze);
    }

    // reset visited
    for (size_t i = 0; i < sz; i++) {
        maze->cells[i].visited = 0;
    }

    // assign start and stop location
    assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}

growingTreeMethods_t strToTreeMethod(const char *str) {
    if (strcmp(str, "newest") == 0) {
        return newestTree;
    }

    if (strcmp(str, "middle") == 0) {
        return middleTree;
    }

    if (strcmp(str, "oldest") == 0) {
        return oldestTree;
    }

    if (strcmp(str, "random") == 0) {
        return randomTree;
    }

    if (strcmp(str, "newest-middle") == 0) {
        return newest_middleTree;
    }

    if (strcmp(str, "newest-oldest") == 0) {
        return newest_oldestTree;
    }

    if (strcmp(str, "newest-random") == 0) {
        return newest_randomTree;
    }

    if (strcmp(str, "middle-oldest") == 0) {
        return middle_oldestTree;
    }

    if (strcmp(str, "middle-random") == 0) {
        return middle_randomTree;
    }

    if (strcmp(str, "oldest-random") == 0) {
        return oldest_randomTree;
    }

    return INVALID_METHOD;
}
