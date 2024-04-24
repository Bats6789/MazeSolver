/* Name: main.c
 * Auth: Blake Wingard - bats23456789@gmail.com
 * Date: 04/24/2024
 * Desc: The main file for Maze Solving
 */

/************************************************************************
 *                              INCLUDES                                *
 ************************************************************************/
#include <getopt.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/************************************************************************
 *                               DEFINES                                *
 ************************************************************************/

/************************************************************************
 *                            STATIC VARS                               *
 ************************************************************************/
static int quite_flag = 0;  // option to silence output

/************************************************************************
 *                       FUNCTION DECLARATIONS                          *
 ************************************************************************/
void help(void);

int print(const char *frmt, ...);
int printError(const char *frmt, ...);
/************************************************************************
 *                                MAIN                                  *
 ************************************************************************/
int main(int argc, char *argv[]) {
    int opt;
    int opts_index = 0;

    // clang-format off
	static struct option long_opts[] = {
		{"quite", no_argument, &quite_flag, 1},
		{0, 0, 0, 0}
	};
	// clang format on

	while ((opt = getopt_long(argc, argv, "hq", long_opts, &opts_index)) != -1) {
		switch (opt) {
			case 0: // long opt
				break;
			case 'h':
				help();
				break;
			case 'q':
				quite_flag = 1;
				break;

            case '?':
				puts("Unrecognized option");
                return EXIT_FAILURE;
                break;

			default:
                puts("help");
                return EXIT_FAILURE;
                break;

		}
	}

	return EXIT_SUCCESS;
}

/************************************************************************
 *                       FUNCTION DEFINITIONS                           *
 ************************************************************************/

void help(void) {
	puts("Usage:");
	puts("  MazeSolver                      TBD");
	puts("");
	puts("Options:");
	puts("  -q, --quite                     Silence all output");
	puts("  -h, --help                      Print this message");
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
