#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "MazeTools.h"
#include "recursiveDivision.h"

static void recursive(Maze_t *maze, Point_t topLeft, Point_t bottomRight) {
	size_t width = bottomRight.x - topLeft.x + 1;
	size_t height = bottomRight.y - topLeft.y + 1;
	size_t x, y;

	if (width == 1 || height == 1) {
		return;
	}

	// The grid should bisect the longest direction.
	// If they are the same, the choice is random.
	if ((width == height && rand() % 2 == 0) || height > width) { // bissect horizontally
		// place walls along the row
		y = rand() % (height - 1) + topLeft.y;

		for (size_t i = topLeft.x; i <= bottomRight.x; i++) {
			maze->cells[y * maze->width + i].bottom = 1;	
			maze->cells[(y + 1) * maze->width + i].top = 1;	
		}

		// remove one wall
		x = rand() % width + topLeft.x;

		maze->cells[y * maze->width + x].bottom = 0;	
		maze->cells[(y + 1) * maze->width + x].top = 0;	

		// resolve two areas
		recursive(maze, topLeft, (Point_t){bottomRight.x, y});

		recursive(maze, (Point_t){topLeft.x, y + 1}, bottomRight);
	} else { // bissect virtically

		// place walls along the collumn
		x = rand() % (width - 1) + topLeft.x;

		for (size_t i = topLeft.y; i <= bottomRight.y; i++) {
			maze->cells[i * maze->width + x].right = 1;	
			maze->cells[i * maze->width + x + 1].left = 1;	
		}

		// remove one wall
		y = rand() % height + topLeft.y;

		maze->cells[y * maze->width + x].right = 0;	
		maze->cells[y * maze->width + x + 1].left = 0;	

		// resolve two areas
		recursive(maze, topLeft, (Point_t){x, bottomRight.y});

		recursive(maze, (Point_t){x + 1, topLeft.y}, bottomRight);
	}
}

static void recursiveSteps(Maze_t *maze, Point_t topLeft, Point_t bottomRight, FILE *restrict stream) {
	size_t width = bottomRight.x - topLeft.x + 1;
	size_t height = bottomRight.y - topLeft.y + 1;
	size_t x, y;

	if (width == 1 || height == 1) {
		return;
	}

	// The grid should bisect the longest direction.
	// If they are the same, the choice is random.
	if ((width == height && rand() % 2 == 0) || height > width) { // bissect horizontally
		// place walls along the row
		y = rand() % (height - 1) + topLeft.y;

		for (size_t i = topLeft.x; i <= bottomRight.x; i++) {
			maze->cells[y * maze->width + i].bottom = 1;	
			maze->cells[(y + 1) * maze->width + i].top = 1;	
		}

		fprintStep(stream, maze);

		// remove one wall
		x = rand() % width + topLeft.x;

		maze->cells[y * maze->width + x].bottom = 0;	
		maze->cells[(y + 1) * maze->width + x].top = 0;	

		fprintStep(stream, maze);

		// resolve two areas
		recursiveSteps(maze, topLeft, (Point_t){bottomRight.x, y}, stream);

		recursiveSteps(maze, (Point_t){topLeft.x, y + 1}, bottomRight, stream);
	} else { // bissect virtically

		// place walls along the collumn
		x = rand() % (width - 1) + topLeft.x;

		for (size_t i = topLeft.y; i <= bottomRight.y; i++) {
			maze->cells[i * maze->width + x].right = 1;	
			maze->cells[i * maze->width + x + 1].left = 1;	
		}

		fprintStep(stream, maze);

		// remove one wall
		y = rand() % height + topLeft.y;

		maze->cells[y * maze->width + x].right = 0;	
		maze->cells[y * maze->width + x + 1].left = 0;	

		fprintStep(stream, maze);

		// resolve two areas
		recursiveSteps(maze, topLeft, (Point_t){x, bottomRight.y}, stream);

		recursiveSteps(maze, (Point_t){x + 1, topLeft.y}, bottomRight, stream);
	}
}

void recursiveDivisionGen(Maze_t *maze) {
	Point_t topLeft = {0, 0};
	Point_t bottomRight = {maze->width - 1, maze->height - 1};

    srand(time(NULL));

	// remove all walls
	for (size_t i = 0; i < maze->width * maze->height; i++) {
		maze->cells[i].left = 0;
		maze->cells[i].right = 0;
		maze->cells[i].top = 0;
		maze->cells[i].bottom = 0;
	}

	for (size_t i = 0; i < maze->width; i++) {
		maze->cells[i].top = 1;
		maze->cells[maze->width * (maze->height - 1) + i].bottom = 1;
	}

	for (size_t i = 0; i < maze->height; i++) {
		maze->cells[maze->width * i].left = 1;
		maze->cells[maze->width * i + maze->width - 1].right = 1;
	}

	recursive(maze, topLeft, bottomRight);

    // assign start and stop location
    assignRandomStartAndStop(maze);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
}

void recursiveDivisionGenWithSteps(Maze_t *maze, FILE *restrict stream) {
	Point_t topLeft = {0, 0};
	Point_t bottomRight = {maze->width - 1, maze->height - 1};

    srand(time(NULL));

	// remove all walls
	for (size_t i = 0; i < maze->width * maze->height; i++) {
		maze->cells[i].left = 0;
		maze->cells[i].right = 0;
		maze->cells[i].top = 0;
		maze->cells[i].bottom = 0;
	}

	for (size_t i = 0; i < maze->width; i++) {
		maze->cells[i].top = 1;
		maze->cells[maze->width * (maze->height - 1) + i].bottom = 1;
	}

	for (size_t i = 0; i < maze->height; i++) {
		maze->cells[maze->width * i].left = 1;
		maze->cells[maze->width * i + maze->width - 1].right = 1;
	}

	fprintStep(stream, maze);

	recursiveSteps(maze, topLeft, bottomRight, stream);

    // assign start and stop location
    assignRandomStartAndStopWithSteps(maze, stream);

    // stringify
    maze->str = graphToString(maze->cells, maze->width, maze->height);
    fputs(maze->str, stream);
}
