/**@file kruskal.h
 * @brief Function prototypes for Kruskal maze generations.
 *
 * This contains only functions concerned with Kruskal's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __KRUSKAL_H__
#define __KRUSKAL_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief Generates a maze using Kruskal's algorithm.
 *
 * @param maze The maze to generate.
 */
void kruskalGen(Maze_t *maze);

/**@brief Generates a maze using Kruskal's algorithm and writes the steps.
 *
 * @param maze The maze to generate.
 * @param stream The stream to write to.
 */
void kruskalGenWithSteps(Maze_t *maze, FILE *restrict stream);

#endif /* ifndef __KRUSKAL_H__ */
