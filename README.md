# systems-project1

Our project is a shell written in C.
## Features implemented
- `exit`: Exits the shell
- `cd`: Changes directories
- `<`: Redirects stdout to a file (overwriting)
- `<<`: Redirects stdout to a file (appending)
- `<`: Redirects stdin from a file (overwriting)
- `<`: Redirects stdin from a file (appending)
- `|`: Redirects stdout from one program to stdin of the next (piping)
- `hist`: Displays the history of commands run, up to 500
## Features attempted
- Parses command line inputs
- Supports any spacing bewteen `;` and `|`
- Supports multipiping
## Bugs/Notes

## Functions
```
int main();

void enter_shell();

void get_commandline();

void input(char * c);

char** split(char * c);

int piping(char *cmd1, char *cmd2);

char* parsedtostr(char **parsed);

void eval(char **parsed);

void INThandler(int sig);

void boldgreen();

void green();

void white();
```
