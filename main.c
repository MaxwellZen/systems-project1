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
	int i = 0;
	char c;
	printf("Enter command: ");
	fflush(stdout);

	while(1) {
		c = getchar();
		if (c=='\n') {
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
			printf("Enter command: ");
			fflush(stdout);
			i = 0;
		} else {
			line[i++]=c;
			line[i]=0;
		}

	}

	return 0;
}
