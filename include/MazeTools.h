/** @file MazeTools.h
 * @brief Function prototypes for dealing with mazes.
 *
 * This contains the prototypes and type definition
 * for dealing with mazes.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */

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

/** @brief Creates a maze from a string
 */
Maze_t createMaze(const char *maze);

Maze_t createMazeWH(size_t width, size_t height);

Maze_t importMaze(FILE *stream);

Point_t findStart(Maze_t maze);

Point_t findStop(Maze_t maze);

Point_t pointShift(Point_t point, Direction_t direction);

Tree_t *getHead(Tree_t *tree);

bool isSameTree(Tree_t *tree1, Tree_t *tree2);

bool solveMaze(Maze_t maze, Point_t start, Point_t stop);

bool solveMazeWithSteps(Maze_t maze, Point_t start, Point_t stop, FILE *stream);

char *graphToString(Cell_t *cells, size_t width, size_t height);

void fprintStep(FILE *stream, Maze_t *maze);

void freeMaze(Maze_t maze);

void generateMaze(Maze_t *maze);

void generateMazeWithSteps(Maze_t *maze, FILE *stream);

void joinTrees(Tree_t *head, Tree_t *node);

#endif /* ifndef __MAZE_TOOLS_H__ */
