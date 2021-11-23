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

// takes string, reads input into the string
void input(char * c) {
	fgets(c, 1000, stdin);
	int n = strlen(c);
	if (c[n-1]=='\n') c[n-1]=0;
}

// takes string, returns parsed input
char** split(char * c) {
  int i = 0;
  int size = 1;

  while (c[i] != '\0') {
    if (c[i] == ' ') size += 1;
    i += 1;
  }

  char ** args = calloc(size, sizeof(char *));
  for (int i = 0; i < size; i ++) {
    args[i] = strsep(&c, " ");
    // printf("%s\n", args[i]);
  }
  return args;
}

// takes arguments, executes commands
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
