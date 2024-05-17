#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"
#include "kruskal.h"

void kruskalGen(Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    size_t edgeCount = 0;
    Edge_t *edges = malloc(sizeof(*edges) * sz * 2);
    Tree_t *trees = malloc(sizeof(*trees) * sz);

    for (size_t i = 0; i < sz; i++) {
        Point_t pt = indexToPoint(i, maze->width);

        if (pt.y > 0) {
            edges[edgeCount].point = pt;
            edges[edgeCount++].dir = up;
        }

        if (pt.x > 0) {
            edges[edgeCount].point = pt;
            edges[edgeCount++].dir = left;
        }

        trees[i] = (Tree_t){i, NULL, NULL, NULL};
    }

    // shuffle
    srand(time(NULL));
    for (size_t j = 0; j < 2; j++) {
        for (size_t i = 0; i < edgeCount; i++) {
            int randI = rand() % edgeCount;
            Edge_t tmp = edges[i];
            edges[i] = edges[randI];
            edges[randI] = tmp;
        }
    }

    for (size_t i = 0; i < edgeCount; i++) {
        Point_t point = edges[i].point;
        size_t i1 = pointToIndex(point, maze->width);
        point = pointShift(point, edges[i].dir);
        size_t i2 = pointToIndex(point, maze->width);

        if (!isSameTree(trees + i1, trees + i2)) {
			mazeConnectCells(maze, i1, i2, edges[i].dir);
            joinTrees(trees + i1, trees + i2);
        }
    }

    free(trees);
    free(edges);

    // assign start and stop location
	assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void kruskalGenWithSteps(Maze_t *maze, FILE *restrict stream) {
    size_t sz = maze->width * maze->height;
    size_t edgeCount = 0;
    Edge_t *edges = malloc(sizeof(*edges) * sz * 2);
    Tree_t *trees = malloc(sizeof(*trees) * sz);

    for (size_t i = 0; i < sz; i++) {
		Point_t pt = indexToPoint(i, maze->width);

        if (pt.y > 0) {
            edges[edgeCount].point = pt;
            edges[edgeCount++].dir = up;
        }

        if (pt.x > 0) {
            edges[edgeCount].point = pt;
            edges[edgeCount++].dir = left;
        }

        trees[i] = (Tree_t){i, NULL, NULL, NULL};
    }

    // shuffle
    srand(time(NULL));
    for (size_t j = 0; j < 2; j++) {
        for (size_t i = 0; i < edgeCount; i++) {
            int randI = rand() % edgeCount;
            Edge_t tmp = edges[i];
            edges[i] = edges[randI];
            edges[randI] = tmp;
        }
    }

    fprintStep(stream, maze);
    for (size_t i = 0; i < edgeCount; i++) {
        Point_t point = edges[i].point;
		size_t i1 = pointToIndex(point, maze->width);
        point = pointShift(point, edges[i].dir);
        size_t i2 = pointToIndex(point, maze->width);

        if (!isSameTree(trees + i1, trees + i2)) {
			mazeConnectCells(maze, i1, i2, edges[i].dir);
            joinTrees(trees + i1, trees + i2);
            fprintStep(stream, maze);
        }
    }

    free(trees);
    free(edges);

    // assign start and stop location
	assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
