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

char s[] = {0xF0, 0x9F, 0x90, 0xA2, '\0'};

void enter_shell() {
  boldgreen();
  printf("%s %s %s ...Entering TURTLE SHELL... %s %s %s\n\n\n", s, s, s, s, s, s);
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

    green();
    printf("%s %s:%s %s$ ", s, strsep(&host, "."), dir, name);
    white();
  }
  else {
    green();
    printf("Enter command: ");
    white();
  }
}

// takes string, returns parsed input
char** split(char * c) {
  int i = 0;
  int dlen = 0;

  for (int i = 0; c[i]; i ++) {
    if (c[i] == ';') dlen += 2;
  }

	char * d = calloc(strlen(c) + dlen, sizeof(char *));
  i = 0;
  int size = 1;
	int count = 0;

  for (int i = 0; c[i]; i ++) {
    if (c[i] == ' ') size += 1;
		else if (c[i] == ';') size += 2;

		if (c[i] == ';') {
			d[count] = ' ';
			d[count + 1] = ';';
			d[count + 2] = ' ';
			count += 3;
		}
		else {
			d[count] = c[i];
			count += 1;
		}
  }
  d[count] = '\0';

  char ** args = calloc(size + 1, sizeof(char *));
  args[size] = NULL;
  count = 0;
  for (int i = 0; i < size; i ++) {
    args[count] = strsep(&d, " ");
    if (strcmp(args[count], "") == 0) count -= 1;
    count += 1;
  }
  return args;
}

// char** split(char * c) {
//   int i = 0;
//   int size = 1;
//
//   while (c[i] != '\0') {
//     if (c[i] == ' ') size += 1;
//     i += 1;
//   }
//
//   char ** args = calloc(size+1, sizeof(char *));
//   for (int i = 0; i < size; i ++) {
//     args[i] = strsep(&c, " ");
//     // printf("%s\n", args[i]);
//   }
//   return args;
// }

int piping(char *cmd1, char *cmd2) {
	FILE *in;
	FILE *out;
    char buff[100];
	in = popen(cmd1, "r");
	out = popen(cmd2, "w");
	while(fgets(buff, sizeof(buff), in)){
		fputs(buff, out);
	}

    pclose(in);
    pclose(out);
	return 0;
}

char* parsedtostr(char **parsed) {
	int parsed_len = 0;
	while(parsed[parsed_len]){
	  parsed_len++;
	}
	int str_len = parsed_len, i;
	for (i = 0; i < parsed_len; i++) str_len += strlen(parsed[i]);

	char *s = calloc(str_len + 1, sizeof(char)); s[0] = 0;
	for (i = 0; i < parsed_len; i++) {
		strcat(s, parsed[i]);
		strcat(s, " ");
	}
	s[str_len] = 0;
	return s;
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
		boldgreen();
		printf("\n%s %s %s ...Exiting TURTLE SHELL... %s %s %s\n\n", s, s, s, s, s, s);
		white();
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

	// everything else??
	else {
		int pipeindex = -1;
		int i;
		for (i = 0; i < parsed_len; i++) {
			if (!strcmp(parsed[i], "|")) {
				pipeindex = i;
				break;
			}
		}
		if (pipeindex > -1) {
			parsed[pipeindex] = 0;
			piping(parsedtostr(parsed), parsedtostr(parsed+pipeindex+1));
		} else {
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
}

void INThandler(int sig) {
  signal(sig, SIG_IGN);
	boldgreen();
	printf("\n\n%s %s %s ...Exiting TURTLE SHELL... %s %s %s\n\n", s, s, s, s, s, s);
	white();
	exit(0);
}

void boldgreen() {
  printf("\033[1;32m");
}

void green() {
  printf("\033[0;32m");
}

void white() {
  printf("\033[0m");
}
