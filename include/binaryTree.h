/**@file binaryTree.h
 * @brief Function prototypes for Binary Tree maze generations.
 *
 * This contains only functions concerned with Binary Tree's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief An enum for selecting a binary tree biases. */
typedef enum {
    northEastTree,        /**@brief Move North-East. */
    northWestTree,        /**@brief Select Move North-West. */
    southEastTree,        /**@brief Select Move South-East. */
    southWestTree,        /**@brief Select Move South-West. */
	INVALID_BIAS          /**@brief Invalid bias selected. */
} binaryTreeBiases_t;

/**@brief Generates a maze using Binary Tree's algorithm.
 *
 * @param maze The maze to generate.
 */
void binaryTreeGen(Maze_t *maze, binaryTreeBiases_t bias);

/**@brief Generates a maze using Binary Tree's algorithm and writes the steps.
 *
 * @param maze The maze to generate.
 * @param stream The stream to write to.
 */
void binaryTreeGenWithSteps(Maze_t *maze, binaryTreeBiases_t bias, FILE *restrict stream);

/**@brief Converts a string to a binary tree bias.
 *
 * @param str The string to convert.
 * @return The method from the string.
 */
binaryTreeBiases_t strToTreeBias(const char *str);

#endif /* ifndef __BINARY_TREE_H__ */
