/**@file recursiveBacktracking.h
 * @brief Function prototypes for Recursive Backtracking maze generations.
 *
 * This contains only functions concerned with Recursive Backtracking algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __RECURSIVE_BACKTRACKING_H__
#define __RECURSIVE_BACKTRACKING_H__

#include <stdio.h>

#include "MazeTools.h"

void recursiveBacktracking(Maze_t *maze);

void recursiveBacktrackingWithSteps(Maze_t *maze, FILE *restrict stream);

#endif /* ifndef __RECURSIVE_BACKTRACKING_H__ */
