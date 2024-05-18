#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "MazeTools.h"
#include "aStar.h"

typedef struct {
    Point_t self;
    Point_t parent;
	uint64_t startDistance;
	uint64_t stopDistance;
} node_t;

typedef struct {
    node_t *nodes;
    size_t sz;
    size_t count;
} queue_t;

static inline uint64_t hueristic(node_t node) {
	return node.startDistance + node.stopDistance;
}

static queue_t initQueue(size_t sz) {
    queue_t queue = {.nodes = NULL, .sz = sz, .count = 0};

    queue.nodes = malloc(sizeof(*queue.nodes) * queue.sz);

    return queue;
}

static void enqueue(queue_t *queue, node_t el) {
	size_t i = 0;

    if (queue->sz == 0) {
        *queue = initQueue(100);
    }

    if (queue->count == queue->sz) {
        queue->sz *= 2;

        queue->nodes = realloc(queue->nodes, sizeof(*queue->nodes) * queue->sz);
    }

	for (i = 0; i < queue->count; i++) {
		if (hueristic(el) < hueristic(queue->nodes[i])) {
			break;
		}
	}

	for (size_t j = queue->count; j > i; j--) {
		queue->nodes[j] = queue->nodes[j - 1];
	}

	queue->nodes[i] = el;
	queue->count++;
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

bool aStarSolve(Maze_t *maze, Point_t start, Point_t stop) {
	size_t dirSz = 0;
	size_t index;
	size_t sz = maze->width * maze->height;
	uint64_t weights[sz];
    queue_t queue = initQueue(100);
    node_t node = {start, start, 0, manhattenDistance(start, stop)};
	node_t map[sz];
	node_t tmpNode;
	Direction_t dir[4];
	bool found = false;

	for (size_t i = 0; i < sz; i++) {
		weights[i] = SIZE_MAX;
	}

	weights[pointToIndex(start, maze->width)] = hueristic(node);

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
			tmpNode.startDistance = node.startDistance + 1;
			for (size_t i = 0; i < dirSz; i++) {
				tmpNode.self = pointShift(node.self, dir[i]);
				tmpNode.stopDistance = manhattenDistance(tmpNode.self, stop);
				index = pointToIndex(tmpNode.self, maze->width);
				if (!maze->cells[index].visited || hueristic(tmpNode) < weights[index]) {
					enqueue(&queue, tmpNode);
					weights[index] = hueristic(tmpNode);
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

bool aStarSolveWithSteps(Maze_t *maze, Point_t start, Point_t stop,
                                FILE *restrict stream) {
	size_t dirSz = 0;
	size_t index;
	size_t sz = maze->width * maze->height;
	uint64_t weights[sz];
    queue_t queue = initQueue(100);
    node_t node = {start, start, 0, manhattenDistance(start, stop)};
	node_t map[sz];
	node_t tmpNode;
	Direction_t dir[4];
	bool found = false;

	for (size_t i = 0; i < sz; i++) {
		weights[i] = SIZE_MAX;
	}

	weights[pointToIndex(start, maze->width)] = 0;

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
			tmpNode.startDistance = node.startDistance + 1;
			for (size_t i = 0; i < dirSz; i++) {
				tmpNode.self = pointShift(node.self, dir[i]);
				tmpNode.stopDistance = manhattenDistance(tmpNode.self, stop);
				index = pointToIndex(tmpNode.self, maze->width);
				if (!maze->cells[index].visited || hueristic(tmpNode) < weights[index]) {
					enqueue(&queue, tmpNode);
					maze->cells[index].queued = 1;
					weights[index] = tmpNode.startDistance;
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


