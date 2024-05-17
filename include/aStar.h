/**@file aStar.h
 * @brief Function prototypes for A-Star maze solving.
 *
 * This contains only functions concerned with A-Star's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __A_STAR_H__
#define __A_STAR_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Solves a maze using A-Star's algorithm.
 *
 * @param maze The maze to solve.
 * @param start The start point of the maze.
 * @param stop The stop point of the maze.
 */
bool aStarSolve(Maze_t *maze, Point_t start, Point_t stop);

/**@brief Solves a maze using A-Star's algorithm and writes the steps.
 *
 * @param maze The maze to solve.
 * @param start The start point of the maze.
 * @param stop The stop point of the maze.
 * @param stream The stream to write to.
 */
bool aStarSolveWithSteps(Maze_t *maze, Point_t start, Point_t stop,
                                FILE *restrict stream);

#endif /* ifndef __A_STAR_H__ */


