/**@file breadthFirst.h
 * @brief Function prototypes for Breadth First maze solving.
 *
 * This contains only functions concerned with Breadth First's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __BREADTH_FIRST_H__
#define __BREADTH_FIRST_H__

#include <stdio.h>
#include <limits.h>

#include "MazeTools.h"

/**@brief Solves a maze using Breadth First's algorithm.
 *
 * @param maze The maze to solve.
 * @param start The start point of the maze.
 * @param stop The stop point of the maze.
 */
bool breadthFirstSolve(Maze_t *maze, Point_t start, Point_t stop);

/**@brief Solves a maze using Breadth First's algorithm and writes the steps.
 *
 * @param maze The maze to solve.
 * @param start The start point of the maze.
 * @param stop The stop point of the maze.
 * @param stream The stream to write to.
 */
bool breadthFirstSolveWithSteps(Maze_t *maze, Point_t start, Point_t stop,
                                FILE *restrict stream);

#endif /* ifndef __BREADTH_FIRST_H__ */
