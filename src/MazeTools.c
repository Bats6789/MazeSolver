#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"
#include "aldous_broder.h"
#include "binaryTree.h"
#include "breadthFirst.h"
#include "depthFirst.h"
#include "eller.h"
#include "growing_tree.h"
#include "huntAndKill.h"
#include "kruskal.h"
#include "prim.h"
#include "recursiveBacktracking.h"
#include "recursiveDivision.h"
#include "sidewinder.h"
#include "wilson.h"

Maze_t createMaze(const char *str) {
    Maze_t maze = {0, 0, NULL, NULL};
    size_t strWidth = 1;
    size_t len = strlen(str);
    size_t rows = 0;
    Point_t point;

    for (size_t i = 0; i < len; i += strWidth) {
        if (str[i] == '\n') {
            rows += 1;
            if (strWidth == 1) {
                maze.width = (i - 1) / 2;
                strWidth = i + 1;
            }
        }
    }

    if (str[len - 2] != '\n') {
        rows += 1;
    }

    maze.height = (rows - 1) / 2;

    maze.cells = malloc(sizeof(*maze.cells) * maze.width * maze.height);

    for (point.y = 0; point.y < maze.height; ++point.y) {
        for (point.x = 0; point.x < maze.width; ++point.x) {
            size_t i = pointToIndex(point, maze.width);
            size_t strI = strWidth * (2 * point.y + 1) + 2 * point.x + 1;

            if (str[strI] == '.' || str[strI] == 's' || str[strI] == 'x') {
                maze.cells[i].visited = 1;
            } else {
                maze.cells[i].visited = 0;
            }

            if (str[strI] == '*') {
                maze.cells[i].path = 1;
                maze.cells[i].visited = 1;
            } else {
                maze.cells[i].path = 0;
            }

            maze.cells[i].queued = str[strI] == 'Q' ? 1 : 0;
            maze.cells[i].observing = str[strI] == ':' ? 1 : 0;

            if (str[strI] == 'S' || str[strI] == 's') {
                maze.cells[i].start = 1;
                maze.cells[i].stop = 0;
            } else if (str[strI] == 'X' || str[strI] == 'x') {
                maze.cells[i].start = 0;
                maze.cells[i].stop = 1;
            } else {
                maze.cells[i].start = 0;
                maze.cells[i].stop = 0;
            }

            maze.cells[i].left = str[strI - 1] == '#' ? 1 : 0;
            maze.cells[i].right = str[strI + 1] == '#' ? 1 : 0;
            maze.cells[i].top = str[strI - strWidth] == '#' ? 1 : 0;
            maze.cells[i].bottom = str[strI + strWidth] == '#' ? 1 : 0;
        }
    }

    maze.str = strdup(str);

    return maze;
}

Maze_t createMazeWH(size_t width, size_t height) {
    Maze_t maze = {width, height, NULL, NULL};
    size_t sz = width * height;

    maze.cells = malloc(sizeof(*maze.cells) * sz);
    if (maze.cells == NULL) {
        perror("Failed to allocate maze");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < sz; i++) {
        maze.cells[i].top = 1;
        maze.cells[i].bottom = 1;
        maze.cells[i].left = 1;
        maze.cells[i].right = 1;
        maze.cells[i].start = 0;
        maze.cells[i].stop = 0;
        maze.cells[i].visited = 0;
        maze.cells[i].path = 0;
        maze.cells[i].observing = 0;
        maze.cells[i].queued = 0;
    }

    return maze;
}

Maze_t importMaze(FILE *stream) {
    char *buf = NULL;
    char c;
    size_t maxSz = 100;
    size_t sz = 0;
    Maze_t maze = {0, 0, NULL, NULL};

    buf = malloc(sizeof(*buf) * maxSz);

    while ((c = fgetc(stream)) != EOF) {
        buf[sz++] = c;

        if (sz >= maxSz) {
            maxSz += 100;
            buf = realloc(buf, sizeof(*buf) * maxSz);
        }
    }

    buf[sz++] = '\0';

    maze = createMaze(buf);

    free(buf);

    return maze;
}

void mazeConnectCells(Maze_t *maze, size_t i1, size_t i2, Direction_t dir) {
    switch (dir) {
        case up:
            maze->cells[i1].top = 0;
            maze->cells[i2].bottom = 0;
            break;
        case down:
            maze->cells[i1].bottom = 0;
            maze->cells[i2].top = 0;
            break;
        case left:
            maze->cells[i1].left = 0;
            maze->cells[i2].right = 0;
            break;
        case right:
            maze->cells[i1].right = 0;
            maze->cells[i2].left = 0;
            break;
    }
}

void mazeBreakWall(Maze_t *maze, Point_t point, Direction_t dir) {
    Point_t point2 = pointShift(point, dir);
    size_t i1 = pointToIndex(point, maze->width);
    size_t i2 = pointToIndex(point2, maze->width);
    mazeConnectCells(maze, i1, i2, dir);
}

Point_t findStart(Maze_t maze) {
    Point_t point;

    for (point.y = 0; point.y < maze.height; point.y++) {
        for (point.x = 0; point.x < maze.width; point.x++) {
            if (maze.cells[pointToIndex(point, maze.width)].start) {
                return point;
            }
        }
    }

    return (Point_t){UINT32_MAX, UINT32_MAX};
}

Point_t findStop(Maze_t maze) {
    Point_t point;

    for (point.y = 0; point.y < maze.height; point.y++) {
        for (point.x = 0; point.x < maze.width; point.x++) {
            if (maze.cells[pointToIndex(point, maze.width)].stop) {
                return point;
            }
        }
    }

    return (Point_t){UINT32_MAX, UINT32_MAX};
}

Point_t pointShift(Point_t point, Direction_t direction) {
    switch (direction) {
        case up:
            return (Point_t){point.x, point.y - 1};
        case down:
            return (Point_t){point.x, point.y + 1};
        case left:
            return (Point_t){point.x - 1, point.y};
        case right:
            return (Point_t){point.x + 1, point.y};
        default:
            return (Point_t){UINT32_MAX, UINT32_MAX};
    }
}

inline size_t pointToIndex(Point_t point, size_t width) {
    return point.y * width + point.x;
}

Point_t indexToPoint(size_t i, size_t width) {
    div_t division = div(i, width);
    return (Point_t){division.rem, division.quot};
}

bool pointEqual(Point_t p1, Point_t p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

Direction_t getRandomDirection(Point_t point, Maze_t maze) {
    Direction_t dir[4];
    size_t dirSz = 0;

    if (point.x > 0) {
        dir[dirSz++] = left;
    }

    if (point.x < maze.width - 1) {
        dir[dirSz++] = right;
    }

    if (point.y > 0) {
        dir[dirSz++] = up;
    }

    if (point.y < maze.height - 1) {
        dir[dirSz++] = down;
    }

    return dir[rand() % dirSz];
}

size_t getRandomDirections(Point_t point, Maze_t maze, Direction_t dir[4]) {
    size_t dirSz = 0;

    if (point.x > 0) {
        dir[dirSz++] = left;
    }

    if (point.x < maze.width - 1) {
        dir[dirSz++] = right;
    }

    if (point.y > 0) {
        dir[dirSz++] = up;
    }

    if (point.y < maze.height - 1) {
        dir[dirSz++] = down;
    }

    for (size_t i = 0; i < 20; i++) {
        size_t left = rand() % dirSz;
        size_t right = rand() % dirSz;

        Direction_t tmp = dir[left];
        dir[left] = dir[right];
        dir[right] = tmp;
    }

    return dirSz;
}

size_t getValidDirections(Point_t point, Maze_t maze, Direction_t dir[4]) {
    size_t dirSz = 0;

    if (point.x > 0) {
        dir[dirSz++] = left;
    }

    if (point.x < maze.width - 1) {
        dir[dirSz++] = right;
    }

    if (point.y > 0) {
        dir[dirSz++] = up;
    }

    if (point.y < maze.height - 1) {
        dir[dirSz++] = down;
    }

    return dirSz;
}

size_t getValidTravelDirections(Point_t point, Maze_t maze, Direction_t dir[4]) {
    size_t dirSz = 0;
	size_t index = pointToIndex(point, maze.width);

    if (point.x > 0 && !maze.cells[index].left) {
        dir[dirSz++] = left;
    }

    if (point.x < maze.width - 1 && !maze.cells[index].right) {
        dir[dirSz++] = right;
    }

    if (point.y > 0 && !maze.cells[index].top) {
        dir[dirSz++] = up;
    }

    if (point.y < maze.height - 1 && !maze.cells[index].bottom) {
        dir[dirSz++] = down;
    }

    return dirSz;
}

Tree_t *getHead(Tree_t *tree) {
    if (!tree) return tree;

    while (tree->parent != NULL) {
        // printf("%p -> %p\n", tree, tree->parent);
        tree = tree->parent;
    }

    return tree;
}

bool isSameTree(Tree_t *tree1, Tree_t *tree2) {
    tree1 = getHead(tree1);
    tree2 = getHead(tree2);

    return tree1->val == tree2->val;
}

bool solveMaze(Maze_t *maze, Point_t start, Point_t stop,
               solveAlgo_t algorithm) {
    bool state = false;
    switch (algorithm) {
        case depthFirst:
            state = depthFirstSolve(maze, start, stop);
			if (maze->str) {
				free(maze->str);
			}
			maze->str = graphToString(maze->cells, maze->width, maze->height);
            break;
        case breadthFirst:
            state = breadthFirstSolve(maze, start, stop);
          break;
        case INVALID_SOLVER:
            break;
        }

    return state;
}

bool solveMazeWithSteps(Maze_t *maze, Point_t start, Point_t stop,
                        solveAlgo_t algorithm, FILE *stream) {
    bool state = false;
    switch (algorithm) {
        case depthFirst:
            state = depthFirstSolveWithSteps(maze, start, stop, stream);
			if (maze->str) {
				free(maze->str);
			}
			maze->str = graphToString(maze->cells, maze->width, maze->height);
			fputs(maze->str, stream);
            break;
        case breadthFirst:
            state = breadthFirstSolveWithSteps(maze, start, stop, stream);
          break;
        case INVALID_SOLVER:
            break;
        }

    return state;
}

static char getCellPathChar(Cell_t cell1, Cell_t cell2) {
    char c = ' ';

    if (cell1.visited && cell2.visited) {
        c = '.';
    }

    if (cell1.path && cell2.path) {
        c = '*';
    }

    return c;
}

char *graphToString(Cell_t *cells, size_t width, size_t height) {
    // In order to stringify, walls will be treated as #, and blanks as ' '
    // There is a border that will surround the maze
    // By treating a cell as a 2x2 grid, and stacking them, we get pretty close
    // However, this would leave the following maze for a 2x2 maze
    // ####
    // # #
    // ####
    // # #
    // Notice that it's missing the right and bottom border
    // So we need to add one more layer to get the following maze
    // #####
    // # # #
    // #####
    // # # #
    // #####
    // Now, because this is a string representation, each line needs a newline
    // So we increase the width by 1. The standard will include the newline for
    // the last line. One more character will be added to the size for the EOS.
    size_t strWidth = width * 2 + 1 + 1;
    size_t strHeight = height * 2 + 1;
    size_t sz = strWidth * strHeight + 1;
    char *str = malloc(sizeof(*str) * sz);
    Point_t point;

    if (str == NULL) {
        perror("Failed to allocate maze");
        exit(EXIT_FAILURE);
    }

    memset(str, '#', sz);  // set everything to walls

    // set EOS
    str[sz - 1] = '\0';

    // set first newline
    str[strWidth - 1] = '\n';

    // setup cells
    for (point.y = 0; point.y < height; ++point.y) {
        for (point.x = 0; point.x < width; ++point.x) {
            size_t i = pointToIndex(point, width);
            size_t strI = strWidth * (2 * point.y + 1) + 2 * point.x + 1;
            if (strI > sz) {
                printf("ERROR: (%u, %u)\n", point.x, point.y);
                exit(EXIT_FAILURE);
            }
            str[strI] = ' ';

            if (cells[i].top == 0) {
                str[strI - strWidth] =
                    getCellPathChar(cells[i], cells[i - width]);
            }

            if (cells[i].bottom == 0) {
                str[strI + strWidth] =
                    getCellPathChar(cells[i], cells[i + width]);
            }

            if (cells[i].left == 0) {
                str[strI - 1] = getCellPathChar(cells[i], cells[i - 1]);
            }

            if (cells[i].right == 0) {
                str[strI + 1] = getCellPathChar(cells[i], cells[i + 1]);
            }

            if (cells[i].queued) {
                str[strI] = cells[i].visited == 1 ? 'q' : 'Q';
            }

            if (cells[i].observing) {
                str[strI] = ':';
            }

            if (cells[i].visited) {
                str[strI] = '.';
            }

            if (cells[i].path) {
                str[strI] = '*';
            }

            if (cells[i].start == 1) {
                str[strI] = cells[i].visited == 1 ? 's' : 'S';
            } else if (cells[i].stop == 1) {
                str[strI] = cells[i].visited == 1 ? 'x' : 'X';
            }

            if (point.x + 1 == width) {
                str[strI + 2] = '\n';
                if (point.y + 1 != height) {
                    str[strI + strWidth + 2] = '\n';
                }
            }
        }
    }

    // last newline
    str[sz - 2] = '\n';

    return str;
}

void fprintStep(FILE *restrict stream, Maze_t *maze) {
    char *str = graphToString(maze->cells, maze->width, maze->height);
    fputs(str, stream);
    fputc('\n', stream);
    free(str);
}

void fprintStepIgnoreVisted(FILE *restrict stream, Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    bool visited[sz];

    for (size_t i = 0; i < sz; i++) {
        visited[i] = maze->cells[i].visited == 1;
        maze->cells[i].visited = 0;
    }

    fprintStep(stream, maze);

    for (size_t i = 0; i < sz; i++) {
        maze->cells[i].visited = visited[i] ? 1 : 0;
    }
}

void freeMaze(Maze_t maze) {
    free(maze.str);
    free(maze.cells);
}

void generateMaze(Maze_t *maze, genAlgo_t algorithm) {
    switch (algorithm) {
        case kruskal:
            kruskalGen(maze);
            break;
        case prim:
            primGen(maze);
            break;
        case back:
            recursiveBacktracking(maze);
            break;
        case aldous_broder:
            aldousBroder(maze);
            break;
        case growing_tree:
            growingTreeGen(maze, newest_randomTree, 0.5);
            break;
        case hunt_and_kill:
            huntAndKillGen(maze);
            break;
        case wilson:
            wilsonGen(maze);
            break;
        case eller:
            ellerGen(maze);
            break;
        case rDivide:
            recursiveDivisionGen(maze);
            break;
        case sidewinder:
            sidewinderGen(maze);
            break;
        case binaryTree:
            binaryTreeGen(maze, southWestTree);
            break;
        case INVALID_ALGORITHM:
            break;
    }
}

void generateMazeWithSteps(Maze_t *maze, genAlgo_t algorithm,
                           FILE *restrict stream) {
    switch (algorithm) {
        case kruskal:
            kruskalGenWithSteps(maze, stream);
            break;
        case prim:
            primGenWithSteps(maze, stream);
            break;
        case back:
            recursiveBacktrackingWithSteps(maze, stream);
            break;
        case aldous_broder:
            aldousBroderWithSteps(maze, stream);
            break;
        case growing_tree:
            growingTreeGenWithSteps(maze, newest_randomTree, 0.5, stream);
            break;
        case hunt_and_kill:
            huntAndKillGenWithSteps(maze, stream);
            break;
        case wilson:
            wilsonGenWithSteps(maze, stream);
            break;
        case eller:
            ellerGenWithSteps(maze, stream);
            break;
        case rDivide:
            recursiveDivisionGenWithSteps(maze, stream);
            break;
        case sidewinder:
            sidewinderGenWithSteps(maze, stream);
            break;
        case binaryTree:
            binaryTreeGenWithSteps(maze, southWestTree, stream);
            break;
        case INVALID_ALGORITHM:
            break;
    }
}

Tree_t *removeNode(Tree_t **head, int val) {
    Tree_t *node = *head;
    Tree_t *left, *right, *parent;

    while (!node && node->val != val) {
        if (node->val < val) {
            node = node->right;
        } else {
            node = node->left;
        }
    }

    if (!node) {
        left = node->left;
        right = node->right;
        parent = node->parent;

        node->parent = NULL;
        node->left = NULL;
        node->right = NULL;

        // node is the head node
        if (!parent) {
            if (!left) {
                *head = left;
                left->parent = NULL;
                joinTrees(*head, right);
            } else {
                right->parent = NULL;
                *head = right;
            }
            // node is the parents left node
        } else if (parent->left == node) {
            parent->left = NULL;
            // node is the parents right node
        } else {
            parent->right = NULL;
        }

        // The next to ifs only run when node was not the head node
        if (parent && !left) {
            left->parent = NULL;
            joinTrees(parent, left);
        }

        if (parent && !right) {
            right->parent = NULL;
            joinTrees(parent, right);
        }
    }

    return node;
}

void joinTrees(Tree_t *head, Tree_t *node) {
    if (!head || !node) {
        return;
    }

    node = getHead(node);

    if (head->val < node->val) {
        if (head->left == NULL) {
            node->parent = head;
            head->left = node;
        } else {
            joinTrees(head->left, node);
        }
    } else if (head->val > node->val) {
        if (head->right == NULL) {
            node->parent = head;
            head->right = node;
        } else {
            joinTrees(head->right, node);
        }
    }
}

void assignRandomStartAndStop(Maze_t *maze) {
    Point_t start, stop;

    if (rand() % 2 == 0) {
        start.x = rand() % maze->width;
        stop.x = rand() % maze->width;
        if (rand() % 2 == 0) {
            start.y = 0;
            stop.y = maze->height - 1;
        } else {
            start.y = maze->height - 1;
            stop.y = 0;
        }
    } else {
        start.y = rand() % maze->height;
        stop.y = rand() % maze->height;
        if (rand() % 2 == 0) {
            start.x = 0;
            stop.x = maze->width - 1;
        } else {
            start.x = maze->width - 1;
            stop.x = 0;
        }
    }

    maze->cells[pointToIndex(start, maze->width)].start = 1;
    maze->cells[pointToIndex(stop, maze->height)].stop = 1;
}

void assignRandomStartAndStopWithSteps(Maze_t *maze, FILE *restrict stream) {
    Point_t start, stop;

    if (rand() % 2 == 0) {
        start.x = rand() % maze->width;
        stop.x = rand() % maze->width;
        if (rand() % 2 == 0) {
            start.y = 0;
            stop.y = maze->height - 1;
        } else {
            start.y = maze->height - 1;
            stop.y = 0;
        }
    } else {
        start.y = rand() % maze->height;
        stop.y = rand() % maze->height;
        if (rand() % 2 == 0) {
            start.x = 0;
            stop.x = maze->width - 1;
        } else {
            start.x = maze->width - 1;
            stop.x = 0;
        }
    }

    maze->cells[pointToIndex(start, maze->width)].start = 1;
    fprintStep(stream, maze);
    maze->cells[pointToIndex(stop, maze->width)].stop = 1;
}

genAlgo_t strToGenAlgo(const char *str) {
    if (strcmp(str, "kruskal") == 0) {
        return kruskal;
    }

    if (strcmp(str, "prim") == 0) {
        return prim;
    }

    if (strcmp(str, "back") == 0) {
        return back;
    }

    if (strcmp(str, "aldous-broder") == 0) {
        return aldous_broder;
    }

    if (strcmp(str, "growing-tree") == 0) {
        return growing_tree;
    }

    if (strcmp(str, "hunt-and-kill") == 0) {
        return hunt_and_kill;
    }

    if (strcmp(str, "wilson") == 0) {
        return wilson;
    }

    if (strcmp(str, "eller") == 0) {
        return eller;
    }

    if (strcmp(str, "divide") == 0) {
        return rDivide;
    }

    if (strcmp(str, "sidewinder") == 0) {
        return sidewinder;
    }

    if (strcmp(str, "binary-tree") == 0) {
        return binaryTree;
    }

    return INVALID_ALGORITHM;
}

solveAlgo_t strToSolveAlgo(const char *str) {
	if (strcmp(str, "depth") == 0) {
		return depthFirst;
	}

	if (strcmp(str, "breadth") == 0) {
		return breadthFirst;
	}
	return INVALID_SOLVER;
}
