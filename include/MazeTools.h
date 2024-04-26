/**@file MazeTools.h
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
 *
 *  All needed memory is allocated to the maze. This includes space for the
 *  string representation (which will be a carbon copy of the string).
 *
 *  @param str The string for creating the maze.
 *  @return The created maze.
 */
Maze_t createMaze(const char *str);

/** @brief Creates a starter maze with specified width and height.
 *
 *  The maze created by this function has all of its walls, no cells have been
 *  visited, no start or stop is assigned, and no path has been found.
 *
 *  @param width The width of the maze.
 *  @param height The height of the maze.
 *  @return The created maze.
 */
Maze_t createMazeWH(size_t width, size_t height);

/**@brief Imports a maze from a stream.
 *
 * This function does not validate if a stream is valid before attempting to
 * read. It is assumed the user has done the necessary error checking before
 * calling this function.
 *
 * @param stream The stream to read for importing.
 */
Maze_t importMaze(FILE *stream);

/**@brief Finds the starting position in the maze.
 *
 * @param maze The maze to search for the starting position.
 * @return The starting position.
 */
Point_t findStart(Maze_t maze);

/**@brief Finds the stopping position in the maze.
 *
 * @param maze The maze to search for the stopping position.
 * @return The stopping position.
 */
Point_t findStop(Maze_t maze);

/**@brief Shifts a point in a direction.
 *
 * Points shifted move one space.
 *
 * @param point The starting location of the point.
 * @param direction The direction to move the point.
 * @return The shifted point.
 */
Point_t pointShift(Point_t point, Direction_t direction);

/**@brief Gets the head of the tree.
 *
 * @param tree The tree's head to get.
 * @return The head of the tree.
 */
Tree_t *getHead(Tree_t *tree);

/**@brief Checks if two trees are the same.
 *
 * Trees are assumed to be the same if their roots are the same key.
 *
 * @param tree1 A tree to check.
 * @param tree2 The other tree to check.
 * @return True if the roots are the same key.
 */
bool isSameTree(Tree_t *tree1, Tree_t *tree2);

/**@brief Solves a maze recursively.
 *
 * This solver uses a simple depth-first flood-fill algorithm.
 *
 * @param maze The maze to solve.
 * @param start The starting location of the solve.
 * @param stop The stopping location of the solve.
 * @return True if the maze was solved.
 */
bool solveMaze(Maze_t maze, Point_t start, Point_t stop);

/**@brief Solves a maze recursively and write the steps.
 *
 * This solver uses a simple depth-first flood-fill algorithm.
 * The function writes all steps taken to solve the maze. Including searching
 * and solving.
 *
 * @param maze The maze to solve.
 * @param start The starting location of the solve.
 * @param stop The stopping location of the solve.
 * @param stream The stream to write to.
 * @return True if the maze was solved.
 */
bool solveMazeWithSteps(Maze_t maze, Point_t start, Point_t stop, FILE *stream);

/**@brief Converts a grid of cells into a string.
 *
 * @param cells The cells to convert into a string.
 * @param width The width of the grid.
 * @param height The height of the grid.
 * @return The string representation of cells.
 */
char *graphToString(Cell_t *cells, size_t width, size_t height);

/**@brief Writes the current state of the maze.
 *
 * @param stream The stream to write to.
 * @param maze The maze to write.
 * @return void
 */
void fprintStep(FILE *stream, Maze_t *maze);

/**@brief Frees a maze.
 *
 * @param maze The maze to free.
 * @return void
 */
void freeMaze(Maze_t maze);

/**@brief Generates a maze.
 *
 * A maze must be properly defined and allocated for this function.
 * This function works by removing walls until a completed maze is generated.
 *
 * @param maze The maze to manipulate.
 * @return void
 */
void generateMaze(Maze_t *maze);

/**@brief Generates a maze and writes the steps.
 *
 * A maze must be properly defined and allocated for this function.
 * This function works by removing walls until a completed maze is generated.
 * This function will write each time a wall is removed.
 *
 * @param maze The maze to manipulate.
 * @param stream The stream to write to.
 * @return void
 */
void generateMazeWithSteps(Maze_t *maze, FILE *stream);

/**@brief Join two trees together.
 *
 * @param head The main tree to receive the new tree.
 * @param node The tree to merge into the head.
 * @return void
 */
void joinTrees(Tree_t *head, Tree_t *node);

#endif /* ifndef __MAZE_TOOLS_H__ */
