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
		// gather input -- after user presses newline, string goes into line
		input(line);

		// split input -- take line and create string array, splitting by space
       		parsed = split(line);
		int parsed_len = 0;
		int i = 0;
		while(parsed[i]){
		  parsed_len++;
		  i++;
		}
		  
		
		if(!strcmp(parsed[0], "exit")){
		  break;
		}

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
				return 0;
			} else {
				int status;
				waitpid(f, &status, 0);
			}
		}

	}

	return 0;
}
