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
	// int i = 0;
	char c;
	history = calloc(500, sizeof(char*));
	h = 0;

	enter_shell();
	white();

	get_commandline();
	fflush(stdout);

	signal(SIGINT, INThandler);
	while(fgets(line, 1000, stdin)) {
		*strchr(line, '\n') = 0;
		if (h >= 500) free(history[h%500]);
		history[h%500] = calloc(1, strlen(line)+1);
		strcpy(history[h%500], line);
		h++;
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
		get_commandline();
		fflush(stdout);
	}

	return 0;
}
