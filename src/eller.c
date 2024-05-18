#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"
#include "eller.h"

static bool isLast(size_t sz, Tree_t *set[sz], size_t i) {
    for (size_t j = i + 1; j < sz; j++) {
        if (isSameTree(set[i], set[j])) {
            return false;
        }
    }

    return true;
}

static bool isInArray(size_t val, size_t sz, int const array[sz]) {
    for (size_t i = 0; i < sz; i++) {
        if (val == array[i]) {
            return true;
        }
    }

    return false;
}

// only true if all branches (and parent) are NULL
static inline bool isIsolated(Tree_t const *node) {
    return !node->parent && !node->left && !node->right;
}

void ellerGen(Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    size_t procCount = 0;
    size_t i, newI;
    Tree_t cells[sz];
    Tree_t *sets[maze->width];
    Point_t point = {0, 0};
    Point_t newPoint;
    int procSets[maze->width];

    for (size_t i = 0; i < sz; i++) {
        cells[i].val = i;
        cells[i].left = NULL;
        cells[i].right = NULL;
        cells[i].parent = NULL;
    }

    // initialize the first row with unique sets
    for (size_t i = 0; i < maze->width; i++) {
        sets[i] = cells + i;
    }

    srand(time(NULL));

    // process all but the last row
    for (point.y = 0; point.y < maze->height - 1; point.y++) {
		procCount = 0;

        // process row
        for (point.x = 0; point.x < maze->width - 1; point.x++) {
            if (!isSameTree(sets[point.x], sets[point.x + 1])) {
                // randomly join
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, right);
                    joinTrees(sets[point.x], sets[point.x + 1]);
                }
            }
        }

        // randomly connect verticals
        for (ssize_t col = point.x; col >= 0; col--) {
            Tree_t *tmp = sets[col];
			point.x = col;

            newPoint = pointShift(point, down);
            i = pointToIndex(point, maze->width);
            newI = pointToIndex(newPoint, maze->width);

            // isolated cells allways extend down
            // If it's the last cell in a set and it hasn't extended,
            // then extend it down
            if (isIsolated(tmp) || (!isInArray(tmp->val, procCount, procSets) &&
                                    isLast(maze->width, sets, col))) {
                mazeBreakWall(maze, point, down);
                joinTrees(cells + i, cells + newI);
                procSets[procCount++] = tmp->val;
                // non-isolated cells sometimes join
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, down);
                    joinTrees(getHead(cells + i), cells + newI);
                    procSets[procCount++] = tmp->val;
                }
            }
        }

        // move to next set
        for (size_t i = 0; i < maze->width; i++) {
            sets[i] = cells + i + (point.y + 1) * maze->width;
        }
    }

    // process the last row
    for (point.x = 0; point.x < maze->width - 1; point.x++) {
        if (!isSameTree(sets[point.x], sets[point.x + 1])) {
            mazeBreakWall(maze, point, right);
            joinTrees(sets[point.x], sets[point.x + 1]);
        }
    }

    // assign start and stop location
    assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void ellerGenWithSteps(Maze_t *maze, FILE *restrict stream) {
    size_t sz = maze->width * maze->height;
    size_t procCount = 0;
    size_t i, newI;
    Tree_t cells[sz];
    Tree_t *sets[maze->width];
    Point_t point = {0, 0};
    Point_t newPoint;
    int procSets[maze->width];

    for (size_t i = 0; i < sz; i++) {
        cells[i].val = i;
        cells[i].left = NULL;
        cells[i].right = NULL;
        cells[i].parent = NULL;
    }

    // initialize the first row with unique sets
    for (size_t i = 0; i < maze->width; i++) {
        sets[i] = cells + i;
    }

	fprintStep(stream, maze);

    srand(time(NULL));

    // process all but the last row
    for (point.y = 0; point.y < maze->height - 1; point.y++) {
		procCount = 0;

        // process row
        for (point.x = 0; point.x < maze->width - 1; point.x++) {
            if (!isSameTree(sets[point.x], sets[point.x + 1])) {
                // randomly join
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, right);
					fprintStep(stream, maze);
                    joinTrees(sets[point.x], sets[point.x + 1]);
                }
            }
        }

        // randomly connect verticals
        for (ssize_t col = point.x; col >= 0; col--) {
            Tree_t *tmp = sets[col];
			point.x = col;

            newPoint = pointShift(point, down);
            i = pointToIndex(point, maze->width);
            newI = pointToIndex(newPoint, maze->width);

            // isolated cells allways extend down
            // If it's the last cell in a set and it hasn't extended,
            // then extend it down
            if (isIsolated(tmp) || (!isInArray(tmp->val, procCount, procSets) &&
                                    isLast(maze->width, sets, col))) {
                mazeBreakWall(maze, point, down);
				fprintStep(stream, maze);
                joinTrees(cells + i, cells + newI);
                procSets[procCount++] = tmp->val;
                // non-isolated cells sometimes join
            } else {
                if (rand() % 2 == 0) {
                    mazeBreakWall(maze, point, down);
					fprintStep(stream, maze);
                    joinTrees(getHead(cells + i), cells + newI);
                    procSets[procCount++] = tmp->val;
                }
            }
        }

        // move to next set
        for (size_t i = 0; i < maze->width; i++) {
            sets[i] = cells + i + (point.y + 1) * maze->width;
        }
    }

    // process the last row
    for (point.x = 0; point.x < maze->width - 1; point.x++) {
        if (!isSameTree(sets[point.x], sets[point.x + 1])) {
            mazeBreakWall(maze, point, right);
			fprintStep(stream, maze);
            joinTrees(sets[point.x], sets[point.x + 1]);
        }
    }

    // assign start and stop location
    assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
