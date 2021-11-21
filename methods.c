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
