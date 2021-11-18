#include <stdio.h>
#include <stdlib.h>
#include <fnctl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include "methods.h"

int main() {
	char line[1000];
	char **parsed;

	while(1) {
		// gather input -- after user presses newline, string goes into line
		input(line);

		// split input -- take line and create string array, splitting by space
		parsed = split(line);

		// if "cd" -- chdir

		// if "exit" -- break out of loop

		// everything else??

	}

	return 0;
}
