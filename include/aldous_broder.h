/**@file aldous_broder.h
 * @brief Function prototypes for Recursive Backtracking maze generations.
 *
 * This contains only functions concerned with Recursive Backtracking algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __ALDOUS_BRODER_H__
#define __ALDOUS_BRODER_H__

#include <stdio.h>

#include "MazeTools.h"

void aldousBroder(Maze_t *maze);

void aldousBroderWithSteps(Maze_t *maze, FILE *restrict stream);

#endif /* ifndef __ALDOUS_BRODER_H__ */
