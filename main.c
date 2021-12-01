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
#include <limits.h>
#include <signal.h>
#include "methods.h"

char **history;
int h;
int f, ischild;
char *homedir;

int main() {
	homedir = getenv("HOME");
	printf("%s\n", homedir);
	char line[1000];
	char **parsed;
	char c;
	history = calloc(500, sizeof(char*));
	h = 0;

	enter_shell();
	white();

	get_commandline();
	fflush(stdout);

	signal(SIGINT, INThandler);
	while(fgets(line, 1000, stdin)) {

		// remove trailing newline
		*strchr(line, '\n') = 0;

		// update history
		if (h >= 500) free(history[h%500]);
		history[h%500] = calloc(1, strlen(line)+1);
		strcpy(history[h%500], line);
		h++;

		// split input
    	parsed = split(line);

		// run commands in between semicolons
		char **cur, **prev;
		for (cur = prev = parsed; *cur; cur++) {
			if (strcmp(*cur, ";")==0) {
				*cur = 0;
				eval(prev);
				prev = cur+1;
			}
		}
		eval(prev);

		// print command line prompt
		get_commandline();
		fflush(stdout);
	}

	print_exit_message();

	return 0;
}
