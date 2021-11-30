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
void INThandler(int);
char **history;
int h;
int f, ischild;

int main() {
	char line[1000];
	char **parsed;
	int i = 0;
	char c;
	history = calloc(500, sizeof(char*));
	h = 0;

	enter_shell();
	white();

	get_commandline();
	fflush(stdout);

	signal(SIGINT, INThandler);
	while(1) {
		c = getchar();
		if (c =='\n') {
			history[h] = calloc(1, strlen(line)+1);
			strcpy(history[h], line);
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
			i = 0;
		} else {
			line[i++]=c;
			line[i]=0;
		}

	}

	return 0;
}
