/**@file main.c
 * @brief The main entry point for the program.
 *
 * This program solves mazes. It is intended for a maze to be provided, but
 * this program does have the ability to generate its own maze for
 * demonstration purposes.
 *
 * @author Blake Wingard (bats23456789@gmail.com)
 * @bug No known bugs.
 */

// clang-format off
/***************************************************************//*******
 *                              INCLUDES                                *
 ************************************************************************/
// clang-format on
#include <errno.h>
#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MazeTools.h"

// clang-format off
/***************************************************************//*******
 *                               DEFINES                                *
 ************************************************************************/
// clang-format on
#define DEFAULT_HEIGHT 10
#define DEFAULT_WIDTH 10

// clang-format off
/**************************************************************//********
 *                            STATIC VARS                               *
 ************************************************************************/
// clang-format on
static int quite_flag = 0;    // option to silence output
static int verbose_flag = 0;  // option to print out everything
static int input_flag = 0;    // option to read a maze from a file

// clang-format off
/***************************************************************//*******
 *                       FUNCTION DECLARATIONS                          *
 ************************************************************************/
// clang-format on
/**@brief Prints the help message for the program.
 *
 * @return void
 */
void help(void);

/**@brief Prints only if quite_flag is not set.
 *
 * The primary purpose of this function is to make disabling printing easier.
 * This is accomplished with the global variable quite_flag. If quite_flag is
 * set to 1, then no printing occurs.
 *
 * @param frmt The format specifier in printf fashion.
 * @param ... The variables to print based on frmt.
 * @return The number of items printed.
 */
int print(const char *frmt, ...);

/**@brief Prints ERROR only if quite_flag is not set.
 *
 * The primary purpose of this function is to make disabling printing easier.
 * This is accomplished with the global variable quite_flag. If quite_flag is
 * set to 1, then no printing occurs.
 * And printing done by this function is done to stderr as opposed to stdout.
 *
 * @param frmt The format specifier in printf fashion.
 * @param ... The variables to print based on frmt.
 * @return The number of items printed.
 */
int printError(const char *frmt, ...);

// clang-format off
/****************************************************************//******
 *                                MAIN                                  *
 ************************************************************************/
// clang-format on
int main(int argc, char *argv[]) {
    int opt = 0;
    int opts_index = 0;
    Maze_t maze = {0, 0, NULL, NULL};
    size_t height = DEFAULT_HEIGHT;
    size_t width = DEFAULT_HEIGHT;
    Point_t start = {0, 0};
    Point_t stop = {0, 0};
    FILE *inFile = NULL;
    FILE *outFile = stdout;
    FILE *stepFile = NULL;

    // clang-format off
	static struct option long_opts[] = {
		{"input", required_argument, NULL, 'i'},
		{"quite", no_argument, &quite_flag, 1},
		{"output", required_argument, NULL, 'o'},
		{"verbose", optional_argument, NULL, 'v'},
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}
	};
	// clang format on

	// parse user arguments
	while ((opt = getopt_long(argc, argv, "hi:qo:v::", long_opts, &opts_index)) != -1) {
		switch (opt) {
			case 0: // long opt
				break;

			case 'h':
				help();
				return EXIT_SUCCESS;
				break;

			case 'i':
                inFile = fopen(optarg, "r");
                if (!inFile) {
                    printError("ERROR opening \"%s\": %s", optarg,
                               strerror(errno));
                    return EXIT_FAILURE;
                }
				input_flag = 1;
				break;

			case 'q':
				quite_flag = 1;
				break;

            case 'o':
                outFile = fopen(optarg, "w");
                if (!outFile) {
                    printError("ERROR opening \"%s\": %s", optarg,
                               strerror(errno));
                    return EXIT_FAILURE;
                }
                break;

			case 'v':
				if (optarg == NULL && optind < argc && argv[optind][0] != '-'){
					optarg = argv[optind++];
				}
                if (!optarg) {
                    stepFile = fopen("steps.step", "w");
                    if (!stepFile) {
                        printError("ERROR opening \"steps.step\": %s",
                                   strerror(errno));
                        return EXIT_FAILURE;
                    }
                } else {
                    stepFile = fopen(optarg, "w");
                    if (!stepFile) {
                        printError("ERROR opening \"%s\": %s", optarg,
                                   strerror(errno));
                        return EXIT_FAILURE;
                    }
                }
                verbose_flag = 1;
                break;

            case '?':
				puts("Unrecognized option");
				help();
                return EXIT_FAILURE;
                break;

			default:
				help();
                return EXIT_FAILURE;
                break;

		}
	}

	// if no input file is provided, attempt to get the size of the maze
	if (input_flag == 0) {
		if (argv[optind] == NULL) {
			print("Using default options (height = %d, width = %d)\n", height,
		 width);
		} else if (optind + 2 > argc) {
			printError("Not enough arguments");
			return EXIT_FAILURE;
		} else {
			char *tmp;
			height = strtoull(argv[optind++], &tmp, 10);
			if (height == 0) {
				if (errno != 0) {
					printError("ERROR: %s\n", strerror(errno));
					return EXIT_FAILURE;
				} else if (argv[optind - 1] == tmp) {
					printError("Invalid value {%s} received\n", argv[optind - 1]);
					return EXIT_FAILURE;
				}
			}
			width = strtoull(argv[optind++], &tmp, 10);
			if (width == 0) {
				if (errno != 0) {
					printError("ERROR: %s\n", strerror(errno));
					return EXIT_FAILURE;
				} else if (argv[optind - 1] == tmp) {
					printError("Invalid value {%s} received\n", argv[optind - 1]);
					return EXIT_FAILURE;
				}
			}
		}
		fflush(stdout);
	}

	// Import or generate maze
	if (input_flag) {
		maze = importMaze(inFile);
		fclose(inFile);
	} else {
		maze = createMazeWH(width, height);
		generateMaze(&maze);
	}

	// Find critical points
	start = findStart(maze);
	stop = findStop(maze);

	// solve maze
	if (verbose_flag) {
		solveMazeWithSteps(maze, start, stop, stepFile);
	} else {
		solveMaze(maze, start, stop);
	}

	// output the results
	free(maze.str);
	maze.str = graphToString(maze.cells, maze.width, maze.height);

	fprintf(outFile, "%s", maze.str);

	// cleanup
	if (verbose_flag) {
		fclose(stepFile);
	}

	freeMaze(maze);

	return EXIT_SUCCESS;
}

// clang-format off
/*****************************************************************//*****
 *                       FUNCTION DEFINITIONS                           *
 ************************************************************************/
// clang-format on

void help(void) {
    // clang-format off
	puts("Usage:");
	puts("  MazeSolver                      Generates a 10x10 maze and solves it");
	puts("  MazeSolver [W] [H]              Generates a maze of size WxH and solves it");
	puts("  MazeSolver [options] [W] [H]    Generates a maze of size WxH and solves it");
	puts("");
	puts("Options:");
	puts("  -i <file>, --input <file>       Import a maze from <file>");
	puts("  -q, --quite                     Silence all output");
	puts("  -o <file>, --output <file>      Output solved maze to <file>");
	puts("  -v [file], --verbose [file]     Send each step for solving to <file>");
	puts("  -h, --help                      Print this message");
    // clang-format on
}

int printError(const char *frmt, ...) {
    va_list args;
    int results = 0;

    va_start(args, frmt);
    if (!quite_flag) {
        results = vfprintf(stderr, frmt, args);
    }
    va_end(args);

    return results;
}

int print(const char *frmt, ...) {
    va_list args;
    int results = 0;

    va_start(args, frmt);
    if (!quite_flag) {
        results = vprintf(frmt, args);
    }
    va_end(args);

    return results;
}
