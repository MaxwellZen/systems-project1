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

void eval(char **parsed) {

	// calculate number of args
	int parsed_len = 0;
	while(parsed[parsed_len]){
	  parsed_len++;
	}
	if (parsed_len == 0) return;

	// exit -- exit program
	if(!strcmp(parsed[0], "exit")){
	  exit(0);
	}

	// cd -- call chdir
	else if(!strcmp(parsed[0], "cd")){
	  if(parsed_len != 2)
		printf("Please follow the format: cd <path>");
	  else{
		int n = chdir(parsed[1]);
		if(n)
		  printf("cd failed: %s", strerror(errno));
	  }
	}

	// everything else??
	else {
		int f = fork();
		if (!f) {
			if (execvp(parsed[0], parsed) == -1) {
				printf("ERROR : %s\n", strerror(errno));
			}
			exit(0);
		} else {
			int status;
			waitpid(f, &status, 0);
		}
	}
}

int main() {
	char line[1000];
	char **parsed;

	while(1) {
		printf("Enter command: ");

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
