/**@file huntAndKill.h
 * @brief Function prototypes for Hunt-and-Kill maze generations.
 *
 * This contains only functions concerned with Hunt-and-Kill algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __HUNT_AND_KILL_H__
#define __HUNT_AND_KILL_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Generates a maze using Hunt-and-Kill algorithm.
 *
 * @param maze The maze to generate.
 */
void huntAndKillGen(Maze_t *maze);

/**@brief Generates a maze using Hunt-and-Kill algorithm and writes the steps.
 *
 * @param maze The maze to generate.
 * @param stream The stream to write to.
 */
void huntAndKillGenWithSteps(Maze_t *maze, FILE *restrict stream);

#endif /* ifndef __HUNT_AND_KILL_H__ */
