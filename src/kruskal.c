#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"
#include "kruskal.h"

void kruskalGen(Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    Edge_t *edges = malloc(sizeof(*edges) * sz * 2);
    size_t edgeCount = 0;
    Tree_t *trees = malloc(sizeof(*trees) * sz);
    Point_t start, stop;

    for (size_t i = 0; i < sz; i++) {
        div_t division = div(i, maze->width);
        Point_t pt = {division.rem, division.quot};

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
        size_t i1 = point.y * maze->width + point.x;
        point = pointShift(point, edges[i].dir);
        size_t i2 = point.y * maze->width + point.x;

        if (!isSameTree(trees + i1, trees + i2)) {
            switch (edges[i].dir) {
                case up:
                    maze->cells[i1].top = 0;
                    maze->cells[i2].bottom = 0;
                    break;
                case down:
                    maze->cells[i1].bottom = 0;
                    maze->cells[i2].top = 0;
                    break;
                case left:
                    maze->cells[i1].left = 0;
                    maze->cells[i2].right = 0;
                    break;
                case right:
                    maze->cells[i1].right = 0;
                    maze->cells[i2].left = 0;
                    break;
            }

            joinTrees(trees + i1, trees + i2);
        }
    }

    free(trees);
    free(edges);

    // assign start and stop location
    if (rand() % 2 == 0) {
        start.x = rand() % maze->width;
        stop.x = rand() % maze->width;
        if (rand() % 2 == 0) {
            start.y = 0;
            stop.y = maze->height - 1;
        } else {
            start.y = maze->height - 1;
            stop.y = 0;
        }
    } else {
        start.y = rand() % maze->height;
        stop.y = rand() % maze->height;
        if (rand() % 2 == 0) {
            start.x = 0;
            stop.x = maze->width - 1;
        } else {
            start.x = maze->width - 1;
            stop.x = 0;
        }
    }

    maze->cells[start.y * maze->width + start.x].start = 1;
    maze->cells[stop.y * maze->width + stop.x].stop = 1;

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void kruskalGenWithSteps(Maze_t *maze, FILE *restrict stream) {
    size_t sz = maze->width * maze->height;
    Edge_t *edges = malloc(sizeof(*edges) * sz * 2);
    size_t edgeCount = 0;
    Tree_t *trees = malloc(sizeof(*trees) * sz);
    Point_t start, stop;

    for (size_t i = 0; i < sz; i++) {
        div_t division = div(i, maze->width);
        Point_t pt = {division.rem, division.quot};

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
        size_t i1 = point.y * maze->width + point.x;
        point = pointShift(point, edges[i].dir);
        size_t i2 = point.y * maze->width + point.x;

        if (!isSameTree(trees + i1, trees + i2)) {
            switch (edges[i].dir) {
                case up:
                    maze->cells[i1].top = 0;
                    maze->cells[i2].bottom = 0;
                    break;
                case down:
                    maze->cells[i1].bottom = 0;
                    maze->cells[i2].top = 0;
                    break;
                case left:
                    maze->cells[i1].left = 0;
                    maze->cells[i2].right = 0;
                    break;
                case right:
                    maze->cells[i1].right = 0;
                    maze->cells[i2].left = 0;
                    break;
            }

            joinTrees(trees + i1, trees + i2);
            fprintStep(stream, maze);
        }
    }

    free(trees);
    free(edges);

    // assign start and stop location
    if (rand() % 2 == 0) {
        start.x = rand() % maze->width;
        stop.x = rand() % maze->width;
        if (rand() % 2 == 0) {
            start.y = 0;
            stop.y = maze->height - 1;
        } else {
            start.y = maze->height - 1;
            stop.y = 0;
        }
    } else {
        start.y = rand() % maze->height;
        stop.y = rand() % maze->height;
        if (rand() % 2 == 0) {
            start.x = 0;
            stop.x = maze->width - 1;
        } else {
            start.x = maze->width - 1;
            stop.x = 0;
        }
    }

    maze->cells[start.y * maze->width + start.x].start = 1;
    fprintStep(stream, maze);
    maze->cells[stop.y * maze->width + stop.x].stop = 1;

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
