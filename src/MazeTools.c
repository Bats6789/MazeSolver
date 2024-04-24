#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"

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
    }

    return maze;
}

Maze_t createMaze(const char *mazeStr) {
    Maze_t maze = {0, 0, NULL};
    size_t len = strlen(mazeStr);
    size_t count = 0;

    maze.str = malloc(sizeof(*maze.str) * len);
    if (maze.str == NULL) {
        perror("Failed to allocate maze");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < len; ++i) {
        if (mazeStr[i] == '\n') {
            if (maze.width == 0) {
                maze.width = count;
            }
            maze.height++;
            continue;
        }

        maze.str[count++] = mazeStr[i];
    }

    if (mazeStr[len - 1] != '\n') {
        if (maze.width == 0) {
            maze.width = count;
        }
        maze.height++;
    }

    maze.str[count] = '\0';

    maze.str = realloc(maze.str, sizeof(*maze.str) * count);
    if (maze.str == NULL) {
        perror("Failed to allocate maze");
        exit(EXIT_FAILURE);
    }

    return maze;
}

void freeMaze(Maze_t maze) { 
	free(maze.str);
	free(maze.cells);
}

void generateMaze(Maze_t *maze) {
    size_t sz = maze->width * maze->height;
    Edge_t *edges = malloc(sizeof(*edges) * sz * 2);
    size_t edgeCount = 0;
    Tree_t *trees = malloc(sizeof(*trees) * sz);
	Point_t start, stop;

    for (size_t i = 0; i < sz; i++) {
        div_t division = div(i, maze->width);
        Point_t pt = {division.rem, division.quot};

        if (pt.y > 0) {
            edges[edgeCount].point = pt;
            edges[edgeCount++].dir = up;
        }

        if (pt.x > 0) {
            edges[edgeCount].point = pt;
            edges[edgeCount++].dir = left;
        }

        trees[i] = (Tree_t){i, NULL, NULL, NULL};
    }

    // shuffle
    srand(time(NULL));
    for (size_t j = 0; j < 2; j++) {
        for (size_t i = 0; i < edgeCount; i++) {
            int randI = rand() % edgeCount;
            Edge_t tmp = edges[i];
            edges[i] = edges[randI];
            edges[randI] = tmp;
        }
    }

    for (size_t i = 0; i < edgeCount; i++) {
        Point_t point = edges[i].point;
        size_t i1 = point.y * maze->width + point.x;
        point = pointShift(point, edges[i].dir);
        size_t i2 = point.y * maze->width + point.x;

		// printf("Edge %zu: (%zu, %zu)\n", i, i1, i2);

        if (!isSameTree(trees + i1, trees + i2)) {
            switch (edges[i].dir) {
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

            joinTrees(trees + i1, trees + i2);
        }
    }

    free(trees);
    free(edges);

	// assign start and stop location
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

	maze->cells[start.y * maze->width + start.x].start = 1;
	maze->cells[stop.y * maze->width + stop.x].stop = 1;

	// stringify
	maze->str = graphToString(maze->cells, maze->width, maze->height);
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
    } else {
        if (head->right == NULL) {
            node->parent = head;
            head->right = node;
        } else {
            joinTrees(head->right, node);
        }
    }
}

bool isSameTree(Tree_t *tree1, Tree_t *tree2) {
    tree1 = getHead(tree1);
    tree2 = getHead(tree2);

    return tree1->val == tree2->val;
}

Tree_t *getHead(Tree_t *tree) {
    if (!tree) return tree;

    while (tree->parent != NULL) {
        tree = tree->parent;
    }

    return tree;
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

	if (str == NULL) {
        perror("Failed to allocate maze");
        exit(EXIT_FAILURE);
	}

	memset(str, '#', sz); // set everything to walls
	
	// set EOS
	str[sz - 1] = '\0';

	// set first newline
	str[strWidth - 1] = '\n';

	// setup cells
	for (size_t y = 0; y < height; ++y) {
		for (size_t x = 0; x < width; ++x) {
			size_t i = y * width + x;
			size_t strI = strWidth * (2 * y + 1) + 2 * x + 1;
			if (strI > sz) {
				printf("ERROR: (%zu, %zu)\n", x, y);
				exit(EXIT_FAILURE);
			}
			str[strI] = ' ';

			if (cells[i].top == 0) {
				str[strI - strWidth] = ' ';
			}

			if (cells[i].bottom == 0) {
				str[strI + strWidth] = ' ';
			}

			if (cells[i].left == 0) {
				str[strI - 1] = ' ';
			}

			if (cells[i].right == 0) {
				str[strI + 1] = ' ';
			}

			if (cells[i].start == 1) {
				str[strI] = 'S';
			} else if (cells[i].stop == 1) {
				str[strI] = 'X';
			}

			if (x + 1 == width) {
				str[strI + 2] = '\n';
				if (y + 1 != height) {
					str[strI + strWidth + 2] = '\n';
				}
			}
		}
	}

	// last newline
	str[sz - 2] = '\n';

	return str;
}

void generateMazeWithSteps(Maze_t *maze, FILE *stream) {
    size_t sz = maze->width * maze->height;
    Edge_t *edges = malloc(sizeof(*edges) * sz * 2);
    size_t edgeCount = 0;
    Tree_t *trees = malloc(sizeof(*trees) * sz);
	Point_t start, stop;

    for (size_t i = 0; i < sz; i++) {
        div_t division = div(i, maze->width);
        Point_t pt = {division.rem, division.quot};

        if (pt.y > 0) {
            edges[edgeCount].point = pt;
            edges[edgeCount++].dir = up;
        }

        if (pt.x > 0) {
            edges[edgeCount].point = pt;
            edges[edgeCount++].dir = left;
        }

        trees[i] = (Tree_t){i, NULL, NULL, NULL};
    }

    // shuffle
    srand(time(NULL));
    for (size_t j = 0; j < 2; j++) {
        for (size_t i = 0; i < edgeCount; i++) {
            int randI = rand() % edgeCount;
            Edge_t tmp = edges[i];
            edges[i] = edges[randI];
            edges[randI] = tmp;
        }
    }

	fprintStep(stream, maze);
    for (size_t i = 0; i < edgeCount; i++) {
        Point_t point = edges[i].point;
        size_t i1 = point.y * maze->width + point.x;
        point = pointShift(point, edges[i].dir);
        size_t i2 = point.y * maze->width + point.x;

        if (!isSameTree(trees + i1, trees + i2)) {
            switch (edges[i].dir) {
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

            joinTrees(trees + i1, trees + i2);
			fprintStep(stream, maze);
        }
    }

    free(trees);
    free(edges);

	// assign start and stop location
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

	maze->cells[start.y * maze->width + start.x].start = 1;
	fprintStep(stream, maze);
	maze->cells[stop.y * maze->width + stop.x].stop = 1;

	// stringify
	maze->str = graphToString(maze->cells, maze->width, maze->height);
	fputs(maze->str, stream);
}

void fprintStep(FILE *stream, Maze_t *maze) {
	char *str = graphToString(maze->cells, maze->width, maze->height);
	fputs(str, stream);
	fputc('\n', stream);
	free(str);
}
