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
  char **args = split(c);

}

// takes string, returns parsed input
char** split(char * c) {
  printf("%s\n", c);
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
