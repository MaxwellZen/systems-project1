#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
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
		printf("Enter command: ");
		fflush(stdout);

		// gather input -- after user presses newline, string goes into line
		input(line);

		// split input -- take line and create string array, splitting by space
       	parsed = split(line);

		char **cur, **prev;
		for (cur = prev = parsed; *cur; cur++) {
			if (strcmp(*cur, ";")==0) {
				*cur = 0;
				eval(prev);
				prev = cur+1;
			}
		}
		eval(prev);

	}

	return 0;
}
