#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "MazeTools.h"
#include "breadthFirst.h"

typedef struct {
    Point_t self;
    Point_t parent;
} node_t;

typedef struct {
    node_t *nodes;
    size_t sz;
    size_t count;
} queue_t;

static queue_t initQueue(size_t sz) {
    queue_t queue = {.nodes = NULL, .sz = sz, .count = 0};

    queue.nodes = malloc(sizeof(*queue.nodes) * queue.sz);

    return queue;
}

static void enqueue(queue_t *queue, node_t el) {
    if (queue->sz == 0) {
        *queue = initQueue(100);
    }

    if (queue->count == queue->sz) {
        queue->sz *= 2;

        queue->nodes = realloc(queue->nodes, sizeof(*queue->nodes) * queue->sz);
    }

	queue->nodes[queue->count++] = el;
}

static node_t dequeue(queue_t *queue) {
	Point_t badPoint = {UINT_MAX, UINT_MAX};
	node_t node = {badPoint, badPoint};

    if (queue->count > 0) {
		node = queue->nodes[0];
		queue->count--;
		for (size_t i = 0; i < queue->count; i++) {
			queue->nodes[i] = queue->nodes[i + 1];
		}
    }

	return node;
}

static void freeQueue(queue_t *queue) {
    free(queue->nodes);
    queue->sz = 0;
    queue->count = 0;
}

bool breadthFirstSolve(Maze_t *maze, Point_t start, Point_t stop) {
    queue_t queue = initQueue(100);
    node_t node = {start, start};
	node_t map[maze->width * maze->height];
	node_t tmpNode;
	Direction_t dir[4];
	size_t dirSz = 0;
	size_t index;
	bool found = false;

    enqueue(&queue, node);

	while (queue.count > 0 && !found) {
		node = dequeue(&queue);
		index = pointToIndex(node.self, maze->width);

		maze->cells[index].visited = 1;
		map[index] = node;

		if (pointEqual(node.self, stop)) {
			found = true;
		} else {
			dirSz = getValidTravelDirections(node.self, *maze, dir);

			tmpNode.parent = node.self;
			for (size_t i = 0; i < dirSz; i++) {
				tmpNode.self = pointShift(node.self, dir[i]);
				index = pointToIndex(tmpNode.self, maze->width);
				if (!maze->cells[index].visited) {
					enqueue(&queue, tmpNode);
				}
			}
		}
	}

	if (found) {
		// draw path
		while (!pointEqual(node.self, node.parent)) {
			maze->cells[pointToIndex(node.self, maze->width)].path = 1;
			node = map[pointToIndex(node.parent, maze->width)];
		}
		// include start
		maze->cells[pointToIndex(node.self, maze->width)].path = 1;

		if (maze->str) {
			free(maze->str);
		}
		maze->str = graphToString(maze->cells, maze->width, maze->height);
	}

    freeQueue(&queue);

    return found;
}

bool breadthFirstSolveWithSteps(Maze_t *maze, Point_t start, Point_t stop,
                                FILE *restrict stream) {
    queue_t queue = initQueue(100);
    node_t node = {start, start};
	node_t map[maze->width * maze->height];
	node_t tmpNode;
	Direction_t dir[4];
	size_t dirSz = 0;
	size_t index;
	bool found = false;

	fprintStep(stream, maze);

    enqueue(&queue, node);
	maze->cells[pointToIndex(node.self, maze->width)].queued = 1;

	while (queue.count > 0 && !found) {
		node = dequeue(&queue);
		index = pointToIndex(node.self, maze->width);

		maze->cells[index].visited = 1;
		maze->cells[index].queued = 0;
		map[index] = node;

		if (pointEqual(node.self, stop)) {
			found = true;
		} else {
			dirSz = getValidTravelDirections(node.self, *maze, dir);

			tmpNode.parent = node.self;
			for (size_t i = 0; i < dirSz; i++) {
				tmpNode.self = pointShift(node.self, dir[i]);
				index = pointToIndex(tmpNode.self, maze->width);
				if (!maze->cells[index].visited) {
					enqueue(&queue, tmpNode);
					maze->cells[index].queued = 1;
				}
			}
			fprintStep(stream, maze);
		}
	}

	for (size_t i = 0; i < maze->width * maze->height; i++) {
		maze->cells[i].queued = 0;
	}

	if (found) {
		// draw path
		while (!pointEqual(node.self, node.parent)) {
			maze->cells[pointToIndex(node.self, maze->width)].path = 1;
			node = map[pointToIndex(node.parent, maze->width)];
			fprintStep(stream, maze);
		}
		// include start
		maze->cells[pointToIndex(node.self, maze->width)].path = 1;

		if (maze->str) {
			free(maze->str);
		}

		maze->str = graphToString(maze->cells, maze->width, maze->height);
		fputs(maze->str, stream);
	}

    freeQueue(&queue);

    return found;
}
