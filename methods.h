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

// terminal colors
void boldgreen();
void green();
void white();
