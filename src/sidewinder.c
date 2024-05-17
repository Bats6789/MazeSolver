#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"
#include "sidewinder.h"

void sidewinderGen(Maze_t *maze) {
    Point_t point = {0, 0};
    Point_t tmpPoint;
    size_t runSet[maze->width];
    size_t runCount = 0;

    srand(time(NULL));

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            runSet[runCount++] = pointToIndex(point, maze->width);
            // for the top row, connect all horizontal cells
            if (point.y == 0) {
                if (point.x + 1 < maze->width) {
                    mazeBreakWall(maze, point, right);
                }
            } else if (point.x + 1 == maze->width) {
                tmpPoint = indexToPoint(runSet[rand() % runCount], maze->width);
                mazeBreakWall(maze, tmpPoint, up);
				runCount = 0;
            } else {
				if (rand() % 2 == 0) {
					mazeBreakWall(maze, point, right);
				} else {
					tmpPoint = indexToPoint(runSet[rand() % runCount], maze->width);
					mazeBreakWall(maze, tmpPoint, up);
					runCount = 0;
				}
            }
        }
        runCount = 0;
    }

    // assign start and stop location
    assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void sidewinderGenWithSteps(Maze_t *maze, FILE *restrict stream) {
    Point_t point = {0, 0};
    Point_t tmpPoint;
    size_t runSet[maze->width];
    size_t runCount = 0;

    srand(time(NULL));

	fprintStep(stream, maze);

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            runSet[runCount++] = pointToIndex(point, maze->width);
            // for the top row, connect all horizontal cells
            if (point.y == 0) {
                if (point.x + 1 < maze->width) {
                    mazeBreakWall(maze, point, right);
                }
            } else if (point.x + 1 == maze->width) {
                tmpPoint = indexToPoint(runSet[rand() % runCount], maze->width);
                mazeBreakWall(maze, tmpPoint, up);
				runCount = 0;
            } else {
				if (rand() % 2 == 0) {
					mazeBreakWall(maze, point, right);
				} else {
					tmpPoint = indexToPoint(runSet[rand() % runCount], maze->width);
					mazeBreakWall(maze, tmpPoint, up);
					runCount = 0;
				}
            }
			fprintStep(stream, maze);
        }
        runCount = 0;
    }

    // assign start and stop location
    assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
