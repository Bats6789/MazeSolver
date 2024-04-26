#ifndef __MAZE_TOOLS_H__
#define __MAZE_TOOLS_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum { up, down, left, right } Direction_t;

typedef struct {
    uint32_t x;
    uint32_t y;
} Point_t;

typedef struct {
    Point_t point;
    Direction_t dir;
} Edge_t;

typedef struct Tree_t {
	int val;
	struct Tree_t *parent;
	struct Tree_t *left;
	struct Tree_t *right;
} Tree_t;

typedef struct {
    union {
        uint32_t walls;
        struct {
            unsigned blank : 24;
			unsigned path : 1;
			unsigned start : 1;
			unsigned stop : 1;
            unsigned visited : 1;
            unsigned top : 1;
            unsigned bottom : 1;
            unsigned left : 1;
            unsigned right : 1;
        };
    };
} Cell_t;

typedef struct {
    size_t width;
    size_t height;
    char *str;
    Cell_t *cells;
} Maze_t;

Maze_t createMaze(const char *maze);
Maze_t createMazeWH(size_t width, size_t height);
void generateMaze(Maze_t *maze);
void generateMazeWithSteps(Maze_t *maze, FILE *stream);
char *graphToString(Cell_t *cells, size_t width, size_t height);
void freeMaze(Maze_t maze);
bool isSameTree(Tree_t *tree1, Tree_t *tree2);
void joinTrees(Tree_t *head, Tree_t *node);
Tree_t *getHead(Tree_t *tree);
Point_t pointShift(Point_t point, Direction_t direction);
void fprintStep(FILE *stream, Maze_t *maze);
Maze_t importMaze(FILE *stream);
bool solveMaze(Maze_t maze, Point_t start, Point_t stop);
bool solveMazeWithSteps(Maze_t maze, Point_t start, Point_t stop, FILE *stream);
Point_t findStart(Maze_t maze);
Point_t findStop(Maze_t maze);

#endif /* ifndef __MAZE_TOOLS_H__ */
