/**@file prim.h
 * @brief Function prototypes for Prim maze generations.
 *
 * This contains only functions concerned with Prim's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __PRIM_H__
#define __PRIM_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Generates a maze using Prim's algorithm.
 *
 * @param maze The maze to generate.
 */
void primGen(Maze_t *maze);

/**@brief Generates a maze using Prim's algorithm and writes the steps.
 *
 * @param maze The maze to generate.
 * @param stream The stream to write to.
 */
void primGenWithSteps(Maze_t *maze, FILE *restrict stream);

#endif /* ifndef __PRIM_H__ */
