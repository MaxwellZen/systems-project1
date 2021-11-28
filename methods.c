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
#include "methods.h"

int enter_shell() {
  char s[] = {0xF0, 0x9F, 0x90, 0xA2, '\0'};

  boldgreen();
  printf("%s %s %s...Entering TURTLE SHELL... %s %s %s \n\n\n", s, s, s, s, s, s);
  return 0;
  white();
}

void get_commandline() {
  int i = 0;
  char hostbuffer[256];
  int hostname = gethostname(hostbuffer, sizeof(hostbuffer));
  char * host = hostbuffer;

  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    char * c = cwd;
    char name[100];
    char dir[100];

    char * hold;
    for (int i = 0; c != NULL; i ++) {
      hold = strsep(&c, "/");
      if (i == 2) strcpy(name, hold);
    }
    strcpy(dir, hold);

    boldgreen();
    printf("TURTLE SHELL %s:%s %s$ ", strsep(&host, "."), dir, name);
    white();
  }
  else {
    boldgreen();
    printf("Enter command: ");
    white();
  }
}

// takes string, returns parsed input
// char** split(char * c) {
//   int i = 0;
//   int size = 1;
// 	char * d;
// 	int count = 0;

//   while (c[i] != '\0') {
//     if (c[i] == ' ') size += 1;
// 		else if (c[i] == ';') size += 2;

// 		if (c[i] == ';') {
// 			d[count] = ' ';
// 			d[count + 1] = ';';
// 			d[count + 2] = ' ';
// 			count += 3;
// 		}
// 		else {
// 			d[count] = c[i];
// 			count += 1;
// 		}
//     i += 1;
//   }

//   char ** args = calloc(size, sizeof(char *));
//   for (int i = 0; i < size; i ++) {
//     args[i] = strsep(&d, " ");
//   }
//   return args;
// }

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

int piping(char *cmd1, char **cmds) {
    char buff[100];

	FILE *in = popen(cmd1, "r");
	fgets(buff, sizeof(buff), in);
	FILE *out = popen(cmds[0], "w");
	fputs(buff, out);
	int fd = fileno(out);
	read(fd, buff, sizeof(out));
	cmds = cmds+1;
	while(cmds[0]){
		FILE *out = popen(cmds[0], "w");
		fputs(buff, out);
		int fd = fileno(out);
		read(fd, buff, sizeof(out));
		cmds = cmds+1;
	}

    pclose(in);
    pclose(out);
	return 0;
}


// takes arguments, executes commands
void eval(char **parsed) {

	// calculate number of args
	int parsed_len = 0;
	while(parsed[parsed_len]){
	  parsed_len++;
	}
	printf("%d", parsed_len);
	if (parsed_len == 0) return;

	// exit -- exit program
	if(!strcmp(parsed[0], "exit")){
	  exit(0);
	}

	// cd -- call chdir
	else if(!strcmp(parsed[0], "cd")){
	  if(parsed_len != 2)
		printf("Please follow the format: cd <path>\n");
	  else{
		int n = chdir(parsed[1]);
		if(n)
		  printf("cd failed: %s\n", strerror(errno));
	  }
	}

	// redirection
	// >
	else if (parsed_len>1 && strcmp(parsed[parsed_len-2], ">")==0) {
		int fd = open(parsed[parsed_len-1], O_WRONLY | O_TRUNC | O_CREAT, 0777);
		if (fd==-1) {
			printf("Error: file opening failed\n");
			return;
		}
		int stdoutcopy = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		parsed[parsed_len-2]=0;
		eval(parsed);
		dup2(stdoutcopy, STDOUT_FILENO);
	}
	// >>
	else if (parsed_len>1 && strcmp(parsed[parsed_len-2], ">>")==0) {
		int fd = open(parsed[parsed_len-1], O_WRONLY | O_APPEND | O_CREAT, 0777);
		if (fd==-1) {
			printf("Error: file opening failed\n");
			return;
		}
		int stdoutcopy = dup(STDOUT_FILENO);
		dup2(fd, STDOUT_FILENO);
		parsed[parsed_len-2]=0;
		eval(parsed);
		dup2(stdoutcopy, STDOUT_FILENO);
	}
	// <
	else if (parsed_len>1 && strcmp(parsed[parsed_len-2], "<")==0) {
		int fd = open(parsed[parsed_len-1], O_RDONLY, 0777);
		if (fd==-1) {
			printf("Error: file opening failed\n");
			return;
		}
		int stdincopy = dup(STDIN_FILENO);
		dup2(fd, STDIN_FILENO);
		parsed[parsed_len-2]=0;
		eval(parsed);
		dup2(stdincopy, STDIN_FILENO);
	}

	// piping, only works with piping
	else if(parsed_len > 2 && !strcmp(parsed[1], "|")){
		printf("test\n");
		char **args = calloc(1, sizeof(parsed)/2);
		int i;
		for(i=1; i<parsed_len; i++){
			if(i%2){
				args[i/2] = parsed[i];
			}
			else{
				if(strcmp(parsed[i], "|")){
					printf("Invalid piping syntax");
					break;
				}
			}
		}
		piping(parsed[0], args);
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

void boldgreen() {
  printf("\033[1;32m");
}

void white() {
  printf("\033[0m");
}
