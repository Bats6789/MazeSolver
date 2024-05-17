#include <stdio.h>

#include "MazeTools.h"
#include "depthFirst.h"

bool depthFirstSolve(Maze_t *maze, Point_t start, Point_t stop) {
    size_t i = pointToIndex(start, maze->width);

    maze->cells[i].visited = 1;

    if (start.x == stop.x && start.y == stop.y) {
        maze->cells[i].path = 1;
        return true;
    }

    if (start.y > 0 && maze->cells[i].top == 0 &&
        maze->cells[i - maze->width].visited == 0) {
        if (depthFirstSolve(maze, (Point_t){start.x, start.y - 1}, stop)) {
            maze->cells[i].path = 1;
            return true;
        }
    }

    if (start.y < maze->height - 1 && maze->cells[i].bottom == 0 &&
        maze->cells[i + maze->width].visited == 0) {
        if (depthFirstSolve(maze, (Point_t){start.x, start.y + 1}, stop)) {
            maze->cells[i].path = 1;
            return true;
        }
    }

    if (start.x > 0 && maze->cells[i].left == 0 &&
        maze->cells[i - 1].visited == 0) {
        if (depthFirstSolve(maze, (Point_t){start.x - 1, start.y}, stop)) {
            maze->cells[i].path = 1;
            return true;
        }
    }

    if (start.x < maze->width - 1 && maze->cells[i].right == 0 &&
        maze->cells[i + 1].visited == 0) {
        if (depthFirstSolve(maze, (Point_t){start.x + 1, start.y}, stop)) {
            maze->cells[i].path = 1;
            return true;
        }
    }

    return false;
}

bool depthFirstSolveWithSteps(Maze_t *maze, Point_t start, Point_t stop,
                              FILE *restrict stream) {
    size_t i = pointToIndex(start, maze->width);

    maze->cells[i].visited = 1;
    fprintStep(stream, maze);

    if (start.x == stop.x && start.y == stop.y) {
        maze->cells[i].path = 1;
        fprintStep(stream, maze);
        return true;
    }

    if (start.y > 0 && maze->cells[i].top == 0 &&
        maze->cells[i - maze->width].visited == 0) {
        if (depthFirstSolveWithSteps(maze, (Point_t){start.x, start.y - 1},
                                     stop, stream)) {
            maze->cells[i].path = 1;
            fprintStep(stream, maze);
            return true;
        }
    }

    if (start.y < maze->height - 1 && maze->cells[i].bottom == 0 &&
        maze->cells[i + maze->width].visited == 0) {
        if (depthFirstSolveWithSteps(maze, (Point_t){start.x, start.y + 1},
                                     stop, stream)) {
            maze->cells[i].path = 1;
            fprintStep(stream, maze);
            return true;
        }
    }

    if (start.x > 0 && maze->cells[i].left == 0 &&
        maze->cells[i - 1].visited == 0) {
        if (depthFirstSolveWithSteps(maze, (Point_t){start.x - 1, start.y},
                                     stop, stream)) {
            maze->cells[i].path = 1;
            fprintStep(stream, maze);
            return true;
        }
    }

    if (start.x < maze->width - 1 && maze->cells[i].right == 0 &&
        maze->cells[i + 1].visited == 0) {
        if (depthFirstSolveWithSteps(maze, (Point_t){start.x + 1, start.y},
                                     stop, stream)) {
            maze->cells[i].path = 1;
            fprintStep(stream, maze);
            return true;
        }
    }

    return false;
}
