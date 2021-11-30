# systems-project1

Our project is a shell written in C.
## Features implemented
- `exit`: Exits the shell
- `cd`: Changes directories (including `~`)
- `<`: Redirects stdout to a file (overwriting)
- `<<`: Redirects stdout to a file (appending)
- `<`: Redirects stdin from a file
- `|`: Redirects stdout from one program to stdin of the next (piping)
- Redirection and piping multiple times, combined
- `hist`: Displays the history of the last 500 commands
- Supports multiple spaces between arguments, and any spacing between `;` and `|`
- Allows quotes to surround text with spaces, ex `git commit -m "commit message"`
- Signal handling: `ctrl+c` ends current command, or exits shell if no command is being run

## Features attempted
- Multipiping was attempted manually but never worked - it currently only works because popen takes care of everything past the first pipe automatically

## Bugs/Notes
- There have been issues with backspace in the past - they might still persist
- Only commands of up to 1000 characters can be processed - something that could be fixed in the future, but hopefully isn't a problem
- Semicolons and pipes don't need to be separated by spaces, but redirection symbols do
- If stdout is redirected multiple times, it'll use the leftmost source given, as opposed to bash which uses the rightmost source given
- On that note, order of operations in general might be thrown off by the fact that redirection operators are processed from right to left

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
