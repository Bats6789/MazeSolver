/**@file wilson.h
 * @brief Function prototypes for Wilson maze generations.
 *
 * This contains only functions concerned with Wilson's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __WILSON_H__
#define __WILSON_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Generates a maze using Wilson's algorithm.
 *
 * @param maze The maze to generate.
 */
void wilsonGen(Maze_t *maze);

/**@brief Generates a maze using Wilson's algorithm and writes the steps.
 *
 * @param maze The maze to generate.
 * @param stream The stream to write to.
 */
void wilsonGenWithSteps(Maze_t *maze, FILE *restrict stream);

#endif /* ifndef __WILSON_H__ */
