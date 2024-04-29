#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"

Maze_t createMaze(const char *str) {
    Maze_t maze = {0, 0, NULL, NULL};
	size_t strWidth = 1;
	size_t len = strlen(str);
	size_t rows = 0;

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

    for (size_t y = 0; y < maze.height; ++y) {
        for (size_t x = 0; x < maze.width; ++x) {
            size_t i = y * maze.width + x;
            size_t strI = strWidth * (2 * y + 1) + 2 * x + 1;

			if (str[strI] == '.') {
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

            if (str[strI] == 'S' ||str[strI] == 's') {
                maze.cells[i].start = 1;
                maze.cells[i].stop = 0;
            } else if (str[strI] == 'X' ||str[strI] == 'x') {
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

Point_t findStart(Maze_t maze) {
	for (uint32_t y = 0; y < maze.height; y++) {
		for (uint32_t x = 0; x < maze.width; x++) {
			if (maze.cells[y * maze.width + x].start) {
				return (Point_t){x, y};
			}
		}
	}

	return (Point_t){UINT32_MAX, UINT32_MAX};
}

Point_t findStop(Maze_t maze) {
	for (uint32_t y = 0; y < maze.height; y++) {
		for (uint32_t x = 0; x < maze.width; x++) {
			if (maze.cells[y * maze.width + x].stop) {
				return (Point_t){x, y};
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

Tree_t *getHead(Tree_t *tree) {
    if (!tree) return tree;

    while (tree->parent != NULL) {
        tree = tree->parent;
    }

    return tree;
}

bool isSameTree(Tree_t *tree1, Tree_t *tree2) {
    tree1 = getHead(tree1);
    tree2 = getHead(tree2);

    return tree1->val == tree2->val;
}

bool solveMaze(Maze_t maze, Point_t start, Point_t stop) {
    size_t i = start.y * maze.width + start.x;

	maze.cells[i].visited = 1;

    if (start.x == stop.x && start.y == stop.y) {
		maze.cells[i].path = 1;
        return true;
    }

    if (start.y > 0 && maze.cells[i].top == 0 &&
        maze.cells[i - maze.width].visited == 0) {
        if (solveMaze(maze, (Point_t){start.x, start.y - 1}, stop)) {
			maze.cells[i].path = 1;
            return true;
        }
    }

    if (start.y < maze.height - 1 && maze.cells[i].bottom == 0 &&
        maze.cells[i + maze.width].visited == 0) {
        if (solveMaze(maze, (Point_t){start.x, start.y + 1}, stop)) {
			maze.cells[i].path = 1;
            return true;
        }
    }

    if (start.x > 0 && maze.cells[i].left == 0 &&
        maze.cells[i - 1].visited == 0) {
        if (solveMaze(maze, (Point_t){start.x - 1, start.y}, stop)) {
			maze.cells[i].path = 1;
            return true;
        }
    }

    if (start.x < maze.width - 1 && maze.cells[i].right == 0 &&
        maze.cells[i + 1].visited == 0) {
        if (solveMaze(maze, (Point_t){start.x + 1, start.y}, stop)) {
			maze.cells[i].path = 1;
            return true;
        }
    }

    return false;
}

bool solveMazeWithSteps(Maze_t maze, Point_t start, Point_t stop, FILE *stream) {
    size_t i = start.y * maze.width + start.x;

	maze.cells[i].visited = 1;
	fprintStep(stream, &maze);

    if (start.x == stop.x && start.y == stop.y) {
		maze.cells[i].path = 1;
		fprintStep(stream, &maze);
        return true;
    }

    if (start.y > 0 && maze.cells[i].top == 0 &&
        maze.cells[i - maze.width].visited == 0) {
        if (solveMazeWithSteps(maze, (Point_t){start.x, start.y - 1}, stop, stream)) {
			maze.cells[i].path = 1;
			fprintStep(stream, &maze);
            return true;
        }
    }

    if (start.y < maze.height - 1 && maze.cells[i].bottom == 0 &&
        maze.cells[i + maze.width].visited == 0) {
        if (solveMazeWithSteps(maze, (Point_t){start.x, start.y + 1}, stop, stream)) {
			maze.cells[i].path = 1;
			fprintStep(stream, &maze);
            return true;
        }
    }

    if (start.x > 0 && maze.cells[i].left == 0 &&
        maze.cells[i - 1].visited == 0) {
        if (solveMazeWithSteps(maze, (Point_t){start.x - 1, start.y}, stop, stream)) {
			maze.cells[i].path = 1;
			fprintStep(stream, &maze);
            return true;
        }
    }

    if (start.x < maze.width - 1 && maze.cells[i].right == 0 &&
        maze.cells[i + 1].visited == 0) {
        if (solveMazeWithSteps(maze, (Point_t){start.x + 1, start.y}, stop, stream)) {
			maze.cells[i].path = 1;
			fprintStep(stream, &maze);
            return true;
        }
    }

    return false;
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
                str[strI - strWidth] = getCellPathChar(cells[i], cells[i - width]);
            }

            if (cells[i].bottom == 0) {
                str[strI + strWidth] = getCellPathChar(cells[i], cells[i + width]);
			}

            if (cells[i].left == 0) {
                str[strI - 1] = getCellPathChar(cells[i], cells[i - 1]);
            }

            if (cells[i].right == 0) {
                str[strI + 1] = getCellPathChar(cells[i], cells[i + 1]);
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

void fprintStep(FILE *stream, Maze_t *maze) {
    char *str = graphToString(maze->cells, maze->width, maze->height);
    fputs(str, stream);
    fputc('\n', stream);
    free(str);
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
