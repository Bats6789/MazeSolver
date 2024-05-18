/**@file sidewinder.h
 * @brief Function prototypes for Sidewinder maze generations.
 *
 * This contains only functions concerned with Sidewinder's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __SIDEWINDER_H__
#define __SIDEWINDER_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Generates a maze using Sidewinder's algorithm.
 *
 * @param maze The maze to generate.
 */
void sidewinderGen(Maze_t *maze);

/**@brief Generates a maze using Sidewinder's algorithm and writes the steps.
 *
 * @param maze The maze to generate.
 * @param stream The stream to write to.
 */
void sidewinderGenWithSteps(Maze_t *maze, FILE *restrict stream);

#endif /* ifndef __SIDEWINDER_H__ */
