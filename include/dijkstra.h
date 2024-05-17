/**@file dijkstra.h
 * @brief Function prototypes for Dijkstra maze solving.
 *
 * This contains only functions concerned with Dijkstra's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __DIJKSTRA_H__
#define __DIJKSTRA_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Solves a maze using Dijkstra's algorithm.
 *
 * @param maze The maze to solve.
 * @param start The start point of the maze.
 * @param stop The stop point of the maze.
 */
bool dijkstraSolve(Maze_t *maze, Point_t start, Point_t stop);

/**@brief Solves a maze using Dijkstra's algorithm and writes the steps.
 *
 * @param maze The maze to solve.
 * @param start The start point of the maze.
 * @param stop The stop point of the maze.
 * @param stream The stream to write to.
 */
bool dijkstraSolveWithSteps(Maze_t *maze, Point_t start, Point_t stop,
                                FILE *restrict stream);

#endif /* ifndef __DIJKSTRA_H__ */

