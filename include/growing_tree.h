/**@file growing_tree.h
 * @brief Function prototypes for Growing Tree maze generations.
 *
 * This contains only functions concerned with Growing Tree's algorithm.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */
#ifndef __GROWING_TREE_H__
#define __GROWING_TREE_H__

#include <stdio.h>

#include "MazeTools.h"

/**@brief An enum for selecting a growing tree method. */
typedef enum {
    newestTree,        /**@brief Select newest cell. */
    middleTree,        /**@brief Select middle cell. */
    oldestTree,        /**@brief Select oldest cell. */
    randomTree,        /**@brief Select random cell. */
    newest_middleTree, /**@brief newest-middle split. */
    newest_oldestTree, /**@brief newest-oldest split. */
    newest_randomTree, /**@brief newest-random split. */
    middle_oldestTree, /**@brief middle-oldest split. */
    middle_randomTree, /**@brief middle-random split. */
    oldest_randomTree, /**@brief oldest-random split. */
	INVALID_METHOD     /**@brief Invalid method selected. */
} growingTreeMethods_t;

/**@brief Generates a maze using Growing Tree algorithm.
 *
 * Growing tree is unique amongst the maze generating algorithms.
 * It works by adding cells to a growing tree, and has several methods
 * for choosing which cell to focus on. In addition, a split can be
 * created between two methods, this allows for a variety of maze
 * types to be generated from a single function.
 *
 * @param maze The maze to generate.
 * @param method The method to add new cells.
 * @param split The ratio between dual-methods.
 */
void growingTreeGen(Maze_t *maze, growingTreeMethods_t method, double split);

/**@brief Generates a maze using Growing Tree algorithm and writes the steps.
 *
 * Growing tree is unique amongst the maze generating algorithms.
 * It works by adding cells to a growing tree, and has several methods
 * for choosing which cell to focus on. In addition, a split can be
 * created between two methods, this allows for a variety of maze
 * types to be generated from a single function.
 *
 * @param maze The maze to generate.
 * @param method The method to add new cells.
 * @param split The ratio between dual-methods.
 * @param stream The stream to write to.
 */
void growingTreeGenWithSteps(Maze_t *maze, growingTreeMethods_t method,
                             double split, FILE *restrict stream);

/**@brief Converts a string to a growing tree method.
 *
 * @param str The string to convert.
 * @return The method from the string.
 */
growingTreeMethods_t strToTreeMethod(const char *str);

#endif /* ifndef __GROWING_TREE_H__ */
