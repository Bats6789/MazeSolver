/**@file MazeTools.h
 * @brief Function prototypes for dealing with mazes.
 *
 * This contains the prototypes and type definition
 * for dealing with mazes.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */

#ifndef __MAZE_TOOLS_H__
#define __MAZE_TOOLS_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/**@brief An enum for keeping track of directions. */
typedef enum {
    up,   /**@brief The up direction. */
    down, /**@brief The down direction. */
    left, /**@brief The left direction. */
    right /**@brief The right direction. */
} Direction_t;

/**@struct Point_t
 * @brief A structure for points.
 *
 * @var Point_t::x
 * The x value of the point.
 *
 * @var Point_t::y
 * The y value of the point.
 */
typedef struct {
    uint32_t x;
    uint32_t y;
} Point_t;

/**@struct Edge_t
 * @brief A structure for edges.
 *
 * @var Edge_t::point
 * The start point of the edge.
 *
 * @var Edge_t::dir
 * The direction the edge is traveling.
 */
typedef struct {
    Point_t point;
    Direction_t dir;
} Edge_t;

/**@struct Tree_t
 * @brief A structure for Trees.
 *
 * @Var Tree_t::val
 * The value of the current node.
 *
 * @var Tree_t::parent
 * The address of the parent node.
 *
 * @var Tree_t::left
 * The address of the left node.
 *
 * @var Tree_t::right
 * The address of the right node.
 */
typedef struct Tree_t {
    int val;
    struct Tree_t *parent;
    struct Tree_t *left;
    struct Tree_t *right;
} Tree_t;

// clang-format off
/**@struct Cell_t
 * @brief A structure for Cells.
 */
typedef struct {
    union {
        /**@brief The value of all properties. */
        uint32_t properties;
        struct {
            unsigned blank : 23;    /**@brief Empty space. */
			unsigned queued : 1;    /**@brief Is the cell enqueued. */
            unsigned observing : 1; /**@brief Is the cell under observation. */
            unsigned path : 1;      /**@brief Is the cell a path to the solution. */
            unsigned start : 1;     /**@brief Is the cell the start. */
            unsigned stop : 1;      /**@brief Is the cell the stop path. */
            unsigned visited : 1;   /**@brief Has the cell been visited. */
            unsigned top : 1;       /**@brief Does the cell contain a wall at the top. */
            unsigned bottom : 1;    /**@brief Does the cell contain a wall at the bottom. */
            unsigned left : 1;      /**@brief Does the cell contain a wall to the left. */
            unsigned right : 1;     /**@brief Does the cell contain a wall to the right. */
        };
    };
} Cell_t;
// clang-format on

/**@struct Maze_t
 * @brief A structure for Mazes.
 *
 * @var Maze_t::width
 * The width of the maze.
 *
 * @var Maze_t::height
 * The height of the maze.
 *
 * @var Maze_t::str
 * The string representation of the maze.
 *
 * @var Maze_t:cells
 * The cells of the maze.
 */
typedef struct {
    size_t width;
    size_t height;
    char *str;
    Cell_t *cells;
} Maze_t;

/**@brief The various kinds of generation algorithms. */
typedef enum {
    kruskal,          /**@brief Kruskal algorithm. */
    prim,             /**@brief Prim algorithm. */
    back,             /**@brief Recursive backtracking algorithm. */
    aldous_broder,    /**@brief Aldous-Broder algorithm. */
    growing_tree,     /**@brief Growing-Tree algorithm. */
    hunt_and_kill,    /**@brief Hunt-and-Kill algorithm. */
    wilson,           /**@brief Wilson's algorithm. */
    eller,            /**@brief Eller's algorithm. */
    rDivide,          /**@brief Recursive Division algorithm. */
    sidewinder,       /**@brief Sidewinder algorithm. */
    binaryTree,       /**@brief Binary Tree algorithm. */
    INVALID_ALGORITHM /**@brief Invalid algorithm. */
} genAlgo_t;

/**@brief The various kinds of generation algorithms. */
typedef enum {
    depthFirst,    /**@brief Depth First algorithm. */
    breadthFirst,  /**@brief Breadth First algorithm. */
    dijkstra,      /**@brief Dijkstra algorithm. */
    aStar,         /**@brief Dijkstra algorithm. */
    INVALID_SOLVER /**@brief Invalid algorithm. */
} solveAlgo_t;

/** @brief Creates a maze from a string
 *
 *  All needed memory is allocated to the maze. This includes space for the
 *  string representation (which will be a carbon copy of the string).
 *
 *  @param str The string for creating the maze.
 *  @return The created maze.
 */
Maze_t createMaze(const char *str);

/** @brief Creates a starter maze with specified width and height.
 *
 *  The maze created by this function has all of its walls, no cells have been
 *  visited, no start or stop is assigned, and no path has been found.
 *
 *  @param width The width of the maze.
 *  @param height The height of the maze.
 *  @return The created maze.
 */
Maze_t createMazeWH(size_t width, size_t height);

/**@brief Imports a maze from a stream.
 *
 * This function does not validate if a stream is valid before attempting to
 * read. It is assumed the user has done the necessary error checking before
 * calling this function.
 *
 * @param stream The stream to read for importing.
 */
Maze_t importMaze(FILE *stream);

/**@brief Connects two cells together in a direction.
 *
 * @param maze The maze to modify.
 * @param i1 The index of the source cell.
 * @param i2 The index of the destination cell.
 * @param dir The direction to break.
 * @return void
 */
void mazeConnectCells(Maze_t *maze, size_t i1, size_t i2, Direction_t dir);

/**@brief Breaks a wall between two cells in a maze.
 *
 * @param maze The maze to modify.
 * @param point The location of the cell to break.
 * @param dir The direction of the wall to break.
 * @return void
 */
void mazeBreakWall(Maze_t *maze, Point_t point, Direction_t dir);

/**@brief Finds the starting position in the maze.
 *
 * @param maze The maze to search for the starting position.
 * @return The starting position.
 */
Point_t findStart(Maze_t maze);

/**@brief Finds the stopping position in the maze.
 *
 * @param maze The maze to search for the stopping position.
 * @return The stopping position.
 */
Point_t findStop(Maze_t maze);

/**@brief Shifts a point in a direction.
 *
 * Points shifted move one space.
 *
 * @param point The starting location of the point.
 * @param direction The direction to move the point.
 * @return The shifted point.
 */
Point_t pointShift(Point_t point, Direction_t direction);

/**@brief Converts a point to an index.
 *
 * @param point The point to convert.
 * @param width The width of a row.
 * @return The indexed point.
 */
size_t pointToIndex(Point_t point, size_t width);

/**@brief Converts an index to a point.
 *
 * @param i The index to convert.
 * @param width The width of a row.
 * @return The point.
 */
Point_t indexToPoint(size_t i, size_t width);

/**@brief Determines if two points are the same.
 *
 * @param p1 The first point.
 * @param p2 The second point.
 * @return True if the x and y of two points are equal.
 */
bool pointEqual(Point_t p1, Point_t p2);

/**@brief Determines the euclid distance between two points.
 *
 * @param p1 The first point.
 * @param p2 The second point.
 * @return The euclid difference between them.
 */
double euclidDistance(Point_t p1, Point_t p2);

/**@brief Determines the manhatten distance between two points.
 *
 * @param p1 The first point.
 * @param p2 The second point.
 * @return The manhatten difference between them.
 */
uint64_t manhattenDistance(Point_t p1, Point_t p2);

/**@brief Provides a random direction to traverse.
 *
 * Note: This function assumes srand() was called prior to it.
 *
 * @param point The point to travel from.
 * @param maze The maze being traversed.
 * @return The direction to traverse.
 */
Direction_t getRandomDirection(Point_t point, Maze_t maze);

/**@brief Provides every direction traversable from a point randomly.
 *
 * Note: This function assumes srand() was called prior to it.
 *
 * @param point The point to travel from.
 * @param maze The maze being traversed.
 * @param dir The directions to traverse.
 * @return The number of traversable directions.
 */
size_t getRandomDirections(Point_t point, Maze_t maze, Direction_t dir[4]);

/**@brief Provides every direction from a point.
 *
 * @param point The point to start from.
 * @param maze The maze being investigated.
 * @param dir The directions to traverse.
 * @return The number of traversable directions.
 */
size_t getValidDirections(Point_t point, Maze_t maze, Direction_t dir[4]);

/**@brief Provides every direction traversable from a point.
 *
 * @param point The point to travel from.
 * @param maze The maze being traversed.
 * @param dir The directions to traverse.
 * @return The number of traversable directions.
 */
size_t getValidTravelDirections(Point_t point, Maze_t maze, Direction_t dir[4]);

/**@brief Gets the head of the tree.
 *
 * @param tree The tree's head to get.
 * @return The head of the tree.
 */
Tree_t *getHead(Tree_t *tree);

/**@brief Checks if two trees are the same.
 *
 * Trees are assumed to be the same if their roots are the same key.
 *
 * @param tree1 A tree to check.
 * @param tree2 The other tree to check.
 * @return True if the roots are the same key.
 */
bool isSameTree(Tree_t *tree1, Tree_t *tree2);

/**@brief Solves a maze recursively.
 *
 * This solver uses a simple depth-first flood-fill algorithm.
 *
 * @param maze The maze to solve.
 * @param start The starting location of the solve.
 * @param stop The stopping location of the solve.
 * @param algorithm The algorithm to solve the maze.
 * @return True if the maze was solved.
 */
bool solveMaze(Maze_t *maze, Point_t start, Point_t stop,
               solveAlgo_t algorithm);

/**@brief Solves a maze recursively and write the steps.
 *
 * This solver uses a simple depth-first flood-fill algorithm.
 * The function writes all steps taken to solve the maze. Including searching
 * and solving.
 *
 * @param maze The maze to solve.
 * @param start The starting location of the solve.
 * @param stop The stopping location of the solve.
 * @param stream The stream for writing.
 * @param algorithm The algorithm to solve the maze.
 * @return True if the maze was solved.
 */
bool solveMazeWithSteps(Maze_t *maze, Point_t start, Point_t stop,
                        solveAlgo_t algorithm, FILE *stream);

/**@brief Converts a grid of cells into a string.
 *
 * @param cells The cells to convert into a string.
 * @param width The width of the grid.
 * @param height The height of the grid.
 * @return The string representation of cells.
 */
char *graphToString(Cell_t *cells, size_t width, size_t height);

/**@brief Writes the current state of the maze.
 *
 * @param stream The stream for writing.
 * @param maze The maze to write.
 * @return void
 */
void fprintStep(FILE *restrict stream, Maze_t *maze);

/**@brief Writes the current state of the maze while ignoring visited.
 *
 * @param stream The stream for writing.
 * @param maze The maze to write.
 * @return void
 */
void fprintStepIgnoreVisted(FILE *restrict stream, Maze_t *maze);

/**@brief Frees a maze.
 *
 * @param maze The maze to free.
 * @return void
 */
void freeMaze(Maze_t maze);

/**@brief Generates a maze.
 *
 * A maze must be properly defined and allocated for this function.
 * This function works by removing walls until a completed maze is generated.
 *
 * @param maze The maze to manipulate.
 * @param algorithm The algorithm used for generation.
 * @return void
 */
void generateMaze(Maze_t *maze, genAlgo_t algorithm);

/**@brief Generates a maze and writes the steps.
 *
 * A maze must be properly defined and allocated for this function.
 * This function works by removing walls until a completed maze is generated.
 * This function will write each time a wall is removed.
 *
 * @param maze The maze to manipulate.
 * @param algorithm The algorithm used for generation.
 * @param stream The stream for writing.
 * @return void
 */
void generateMazeWithSteps(Maze_t *maze, genAlgo_t algorithm,
                           FILE *restrict stream);

/**@brief Removes a node from the tree.
 *
 * The tree's head node will be replaced in the event that the
 * head node is the node desired for removal. If a node is found,
 * the node will be returned after removing.
 *
 * @param head The head of the tree.
 * @param val The value of the node to remove.
 * @return The removed node.
 */
Tree_t *removeNode(Tree_t **head, int val);

/**@brief Join two trees together.
 *
 * @param head The main tree to receive the new tree.
 * @param node The tree to merge into the head.
 * @return void
 */
void joinTrees(Tree_t *head, Tree_t *node);

/**@brief Assigns a random start and stop location in a maze.
 *
 * Note: This function assumes srand() was called prior to it.
 *
 * @param maze The maze to assign the points.
 */
void assignRandomStartAndStop(Maze_t *maze);

/**@brief Assigns a random start and stop location in a maze, and writes it.
 *
 * Note: Only writes the start step. The stop step is left for the user to
 * determine when to write it.
 * This function assumes srand() was called prior to it.
 *
 * @param maze The maze to assign the points.
 * @param stream The stream for writing.
 */
void assignRandomStartAndStopWithSteps(Maze_t *maze, FILE *restrict stream);

/**@brief Convert a string to a algorithm.
 *
 * @param str The string to convert.
 * @return The algorithm converted to.
 */
genAlgo_t strToGenAlgo(const char *str);

/**@brief Convert a string to a algorithm.
 *
 * @param str The string to convert.
 * @return The algorithm converted to.
 */
solveAlgo_t strToSolveAlgo(const char *str);

#endif /* ifndef __MAZE_TOOLS_H__ */
