/**@file recursiveDivision.h
 * @brief Function prototypes for Recursive Division maze generations.
 *
 * This contains only functions concerned with Recursive Division's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __RECURSIVE_DIVISION_H__
#define __RECURSIVE_DIVISION_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Generates a maze using Recursive Division's algorithm.
 *
 * @param maze The maze to generate.
 */
void recursiveDivisionGen(Maze_t *maze);

/**@brief Generates a maze using Recursive Division's algorithm and writes the steps.
 *
 * @param maze The maze to generate.
 * @param stream The stream to write to.
 */
void recursiveDivisionGenWithSteps(Maze_t *maze, FILE *restrict stream);

#endif /* ifndef __RECURSIVE_DIVISION_H__ */
