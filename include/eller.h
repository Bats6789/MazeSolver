/**@file eller.h
 * @brief Function prototypes for Eller maze generations.
 *
 * This contains only functions concerned with Eller's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __ELLER_H__
#define __ELLER_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Generates a maze using Eller's algorithm.
 *
 * @param maze The maze to generate.
 */
void ellerGen(Maze_t *maze);

/**@brief Generates a maze using Eller's algorithm and writes the steps.
 *
 * @param maze The maze to generate.
 * @param stream The stream to write to.
 */
void ellerGenWithSteps(Maze_t *maze, FILE *restrict stream);

#endif /* ifndef __ELLER_H__ */
