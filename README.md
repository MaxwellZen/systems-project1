# systems-project1

Our project is a shell written in C.
## Features implemented
- `exit`: Exits the shell
- `cd`: Changes directories
- `<`: Redirects stdout to a file (overwriting)
- `<<`: Redirects stdout to a file (appending)
- `<`: Redirects stdin from a file
- `|`: Redirects stdout from one program to stdin of the next (piping)
- `hist`: Displays the history of commands run, up to 500
- Supports multiple spaces between arguments, and any spacing between `;` and `|`, ex `ls   -a -l|  wc >  test;  cat   test`
- Allows quotes, ex `git commit -m "commit message"`
- Supports piping
- Signal handling: `ctrl+c` ends current commands

## Features attempted
- Multipiping was attempted manually but entered an infinite loop - it currently works, but only because popen takes care of it automatically

## Bugs/Notes

## Functions
```
// main method
int main();

// entering shell
void enter_shell();

// returns command line
void get_commandline();

// takes string, reads input into the string
void input(char * c);

// takes string, returns parsed input
char** split(char * c);

// takes two strings of commands to run, pipes one into the other
int piping(char *cmd1, char *cmd2);

// takes parsed input, recreates space-separated string
char* parsedtostr(char **parsed);

// takes arguments, executes commands
void eval(char **parsed);

// prints exit shell statement
void INThandler(int sig);

// switches colors
void boldgreen();
void green();
void white();
```
