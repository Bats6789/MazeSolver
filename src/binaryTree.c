#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"
#include "binaryTree.h"

static void binaryTreeSW(Maze_t *maze) {
    Point_t point;

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            if (point.y == 0) {      // can't go up
                if (point.x != 0) {  // can't go left
                    mazeBreakWall(maze, point, left);
                }
            } else if (point.x == 0) {  // can't go left
                mazeBreakWall(maze, point, up);
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, left);
                } else {
                    mazeBreakWall(maze, point, up);
                }
            }
        }
    }
}

static void binaryTreeSE(Maze_t *maze) {
    Point_t point;

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            if (point.y == 0) {                    // can't go up
                if (point.x + 1 != maze->width) {  // can't go right
                    mazeBreakWall(maze, point, right);
                }
            } else if (point.x + 1 == maze->width) {  // can't go right
                mazeBreakWall(maze, point, up);
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, right);
                } else {
                    mazeBreakWall(maze, point, up);
                }
            }
        }
    }
}

static void binaryTreeNW(Maze_t *maze) {
    Point_t point;

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            if (point.y + 1 == maze->height) {     // can't go down
                if (point.x != 0) {  // can't go left
                    mazeBreakWall(maze, point, left);
                }
            } else if (point.x == 0) {  // can't go left
                mazeBreakWall(maze, point, down);
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, left);
                } else {
                    mazeBreakWall(maze, point, down);
                }
            }
        }
    }
}

static void binaryTreeNE(Maze_t *maze) {
    Point_t point;

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            if (point.y + 1 == maze->height) {     // can't go down
                if (point.x + 1 != maze->width) {  // can't go right
                    mazeBreakWall(maze, point, right);
                }
            } else if (point.x + 1 == maze->width) {  // can't go right
                mazeBreakWall(maze, point, down);
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, right);
                } else {
                    mazeBreakWall(maze, point, down);
                }
            }
        }
    }
}

static void binaryTreeStepSW(Maze_t *maze, FILE *restrict stream) {
    Point_t point;

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            if (point.y == 0) {      // can't go up
                if (point.x != 0) {  // can't go left
                    mazeBreakWall(maze, point, left);
                }
            } else if (point.x == 0) {  // can't go left
                mazeBreakWall(maze, point, up);
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, left);
                } else {
                    mazeBreakWall(maze, point, up);
                }
            }
			fprintStep(stream, maze);
        }
    }
}

static void binaryTreeStepSE(Maze_t *maze, FILE *restrict stream) {
    Point_t point;

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            if (point.y == 0) {                    // can't go up
                if (point.x + 1 != maze->width) {  // can't go right
                    mazeBreakWall(maze, point, right);
                }
            } else if (point.x + 1 == maze->width) {  // can't go right
                mazeBreakWall(maze, point, up);
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, right);
                } else {
                    mazeBreakWall(maze, point, up);
                }
            }
			fprintStep(stream, maze);
        }
    }
}

static void binaryTreeStepNW(Maze_t *maze, FILE *restrict stream) {
    Point_t point;

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            if (point.y + 1 == maze->height) {     // can't go down
                if (point.x != 0) {  // can't go left
                    mazeBreakWall(maze, point, left);
                }
            } else if (point.x == 0) {  // can't go left
                mazeBreakWall(maze, point, down);
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, left);
                } else {
                    mazeBreakWall(maze, point, down);
                }
            }
			fprintStep(stream, maze);
        }
    }
}

static void binaryTreeStepNE(Maze_t *maze, FILE *restrict stream) {
    Point_t point;

    for (point.y = 0; point.y < maze->height; point.y++) {
        for (point.x = 0; point.x < maze->width; point.x++) {
            if (point.y + 1 == maze->height) {     // can't go down
                if (point.x + 1 != maze->width) {  // can't go right
                    mazeBreakWall(maze, point, right);
                }
            } else if (point.x + 1 == maze->width) {  // can't go right
                mazeBreakWall(maze, point, down);
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, right);
                } else {
                    mazeBreakWall(maze, point, down);
                }
            }
			fprintStep(stream, maze);
        }
    }
}

void binaryTreeGen(Maze_t *maze, binaryTreeBiases_t bias) {
    srand(time(NULL));

    switch (bias) {
        case northEastTree:
            binaryTreeNE(maze);
            break;
        case northWestTree:
            binaryTreeNW(maze);
            break;
        case southEastTree:
            binaryTreeSE(maze);
            break;
        case southWestTree:
            binaryTreeSW(maze);
            break;
        case INVALID_BIAS:
            break;
    }

    // assign start and stop location
    assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void binaryTreeGenWithSteps(Maze_t *maze, binaryTreeBiases_t bias,
                            FILE *restrict stream) {
    srand(time(NULL));

	fprintStep(stream, maze);

    switch (bias) {
        case northEastTree:
            binaryTreeStepNE(maze, stream);
            break;
        case northWestTree:
            binaryTreeStepNW(maze, stream);
            break;
        case southEastTree:
            binaryTreeStepSE(maze, stream);
            break;
        case southWestTree:
            binaryTreeStepSW(maze, stream);
            break;
        case INVALID_BIAS:
            break;
    }

    // assign start and stop location
    assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}

binaryTreeBiases_t strToTreeBias(const char *str) {
    if (strcmp(str, "northeast") == 0) {
        return northEastTree;
    }

    if (strcmp(str, "northwest") == 0) {
        return northWestTree;
    }

    if (strcmp(str, "southeast") == 0) {
        return southEastTree;
    }

    if (strcmp(str, "southwest") == 0) {
        return southWestTree;
    }

    return INVALID_BIAS;
}
