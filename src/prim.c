#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"
#include "prim.h"

static bool addElement(Tree_t *list, size_t sz, Tree_t element) {
    bool found = false;

    for (size_t i = 0; i < sz && !found; i++) {
        found = list[i].val == element.val;
    }

    if (!found) {
        list[sz] = element;
    }

    return !found;
}


void primGen(Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    size_t startI;
    ssize_t frontierSz = 0;
    Point_t cellPt;
    Tree_t *trees = malloc(sizeof(*trees) * sz);
    Tree_t *frontiers = malloc(sizeof(*trees) * sz);

    for (size_t i = 0; i < sz; i++) {
        trees[i] = (Tree_t){i, NULL, NULL, NULL};
    }

    srand(time(NULL));

    // random starting cell
    startI = rand() % sz;

    // Get initial frontiers
	cellPt = indexToPoint(startI, maze->width);

    if (cellPt.x > 0) {
        frontiers[frontierSz++] = trees[startI - 1];
    }

    if (cellPt.x < maze->width - 1) {
        frontiers[frontierSz++] = trees[startI + 1];
    }

    if (cellPt.y > 0) {
        frontiers[frontierSz++] = trees[startI - maze->width];
    }

    if (cellPt.y < maze->height - 1) {
        frontiers[frontierSz++] = trees[startI + maze->width];
    }

    // evaluate frontiers
    while (frontierSz > 0) {
        ssize_t randI = rand() % frontierSz;
        size_t frontierI = frontiers[randI].val;
        Tree_t potCells[4];
        size_t potSz = 0;
        size_t randPotCellI = 0;
        Point_t cellPt2 = {0, 0};
		Direction_t dir;

		cellPt = indexToPoint(frontierI, maze->width);

        // find cells in maze adjacent to the current frontier cell
        if (cellPt.x > 0) {
            size_t i = frontierI - 1;
            if (isSameTree(trees + startI, trees + i)) {
                potCells[potSz++] = trees[i];
            } else {
                if (addElement(frontiers, frontierSz, trees[i])) {
                    frontierSz++;
                }
            }
        }

        if (cellPt.x < maze->width - 1) {
            size_t i = frontierI + 1;
            if (isSameTree(trees + startI, trees + i)) {
                potCells[potSz++] = trees[i];
            } else {
                if (addElement(frontiers, frontierSz, trees[i])) {
                    frontierSz++;
                }
            }
        }

        if (cellPt.y > 0) {
            size_t i = frontierI - maze->width;
            if (isSameTree(trees + startI, trees + i)) {
                potCells[potSz++] = trees[i];
            } else {
                if (addElement(frontiers, frontierSz, trees[i])) {
                    frontierSz++;
                }
            }
        }

        if (cellPt.y < maze->height - 1) {
            size_t i = frontierI + maze->width;
            if (isSameTree(trees + startI, trees + i)) {
                potCells[potSz++] = trees[i];
            } else {
                if (addElement(frontiers, frontierSz, trees[i])) {
                    frontierSz++;
                }
            }
        }

        // pick a random maze cell
        randPotCellI = potCells[rand() % potSz].val;

        // join edges
		cellPt2 = indexToPoint(randPotCellI, maze->width);


        if (cellPt.x != cellPt2.x) {
			dir = cellPt.x > cellPt2.x ? left : right;
        } else {
			dir = cellPt.y > cellPt2.y ? up : down;
        }

		mazeConnectCells(maze, frontierI, randPotCellI, dir);

        joinTrees(trees + startI, trees + frontierI);

        // remove frontier
        for (ssize_t i = randI; i < frontierSz - 1; i++) {
            frontiers[i] = frontiers[i + 1];
        }
        frontierSz--;
    }

	free(trees);
	free(frontiers);

    // assign start and stop location
	assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void primGenWithSteps(Maze_t *maze, FILE *restrict stream) {
    size_t sz = maze->width * maze->height;
    size_t startI;
    ssize_t frontierSz = 0;
    Point_t cellPt;
    Tree_t *trees = malloc(sizeof(*trees) * sz);
    Tree_t *frontiers = malloc(sizeof(*trees) * sz);

    for (size_t i = 0; i < sz; i++) {
        trees[i] = (Tree_t){i, NULL, NULL, NULL};
    }

    srand(time(NULL));

    // random starting cell
    startI = rand() % sz;

    // Get initial frontiers
	cellPt = indexToPoint(startI, maze->width);

    if (cellPt.x > 0) {
        frontiers[frontierSz++] = trees[startI - 1];
        maze->cells[startI - 1].observing = 1;
    }

    if (cellPt.x < maze->width - 1) {
        frontiers[frontierSz++] = trees[startI + 1];
        maze->cells[startI + 1].observing = 1;
    }

    if (cellPt.y > 0) {
        frontiers[frontierSz++] = trees[startI - maze->width];
        maze->cells[startI - maze->width].observing = 1;
    }

    if (cellPt.y < maze->height - 1) {
        frontiers[frontierSz++] = trees[startI + maze->width];
        maze->cells[startI + maze->width].observing = 1;
    }

    fprintStep(stream, maze);

    // evaluate frontiers
    while (frontierSz != 0) {
        size_t randI = rand() % frontierSz;
        size_t frontierI = frontiers[randI].val;
        Tree_t potCells[4];
        size_t potSz = 0;
        size_t randPotCellI = 0;
        Point_t cellPt2 = {0, 0};
		Direction_t dir;

		cellPt = indexToPoint(frontierI, maze->width);

        // find cells in maze adjacent to the current frontier cell
        if (cellPt.x > 0) {
            size_t i = frontierI - 1;
            if (isSameTree(trees + startI, trees + i)) {
                potCells[potSz++] = trees[i];
            } else {
                if (addElement(frontiers, frontierSz, trees[i])) {
                    frontierSz++;
                    maze->cells[i].observing = 1;
                }
            }
        }

        if (cellPt.x < maze->width - 1) {
            size_t i = frontierI + 1;
            if (isSameTree(trees + startI, trees + i)) {
                potCells[potSz++] = trees[i];
            } else {
                if (addElement(frontiers, frontierSz, trees[i])) {
                    frontierSz++;
                    maze->cells[i].observing = 1;
                }
            }
        }

        if (cellPt.y > 0) {
            size_t i = frontierI - maze->width;
            if (isSameTree(trees + startI, trees + i)) {
                potCells[potSz++] = trees[i];
            } else {
                if (addElement(frontiers, frontierSz, trees[i])) {
                    frontierSz++;
                    maze->cells[i].observing = 1;
                }
            }
        }

        if (cellPt.y < maze->height - 1) {
            size_t i = frontierI + maze->width;
            if (isSameTree(trees + startI, trees + i)) {
                potCells[potSz++] = trees[i];
            } else {
                if (addElement(frontiers, frontierSz, trees[i])) {
                    frontierSz++;
                    maze->cells[i].observing = 1;
                }
            }
        }

        // pick a random maze cell
        randPotCellI = potCells[rand() % potSz].val;

        // join edges
		cellPt2 = indexToPoint(randPotCellI, maze->width);

        if (cellPt.x != cellPt2.x) {
			dir = cellPt.x > cellPt2.x ? left : right;
        } else {
			dir = cellPt.y > cellPt2.y ? up : down;
        }

		mazeConnectCells(maze, frontierI, randPotCellI, dir);
        maze->cells[frontierI].observing = 0;

        joinTrees(trees + startI, trees + frontierI);

        fprintStep(stream, maze);

        // remove frontier
        for (ssize_t i = randI; i < frontierSz - 1; i++) {
            frontiers[i] = frontiers[i + 1];
        }
        frontierSz--;
    }

	free(trees);
	free(frontiers);

    // assign start and stop location
	assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
