/**@file depthFirst.h
 * @brief Function prototypes for Depth First maze solving.
 *
 * This contains only functions concerned with Depth First's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __DEPTH_FIRST_H__
#define __DEPTH_FIRST_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Solves a maze using Depth First's algorithm.
 *
 * @param maze The maze to solve.
 * @param start The start point of the maze.
 * @param stop The stop point of the maze.
 */
bool depthFirstSolve(Maze_t *maze, Point_t start, Point_t stop);

/**@brief Solves a maze using Depth First's algorithm and writes the steps.
 *
 * @param maze The maze to solve.
 * @param start The start point of the maze.
 * @param stop The stop point of the maze.
 * @param stream The stream to write to.
 */
bool depthFirstSolveWithSteps(Maze_t *maze, Point_t start, Point_t stop, FILE *restrict stream);

#endif /* ifndef __DEPTH_FIRST_H__ */
