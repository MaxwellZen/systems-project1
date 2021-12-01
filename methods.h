// prints text upon entering shell
void enter_shell();

// prints command line prompt
void get_commandline();

// prints text upon exiting shell
void print_exit_message();

// takes string, returns parsed input
char** split(char * c);

// takes two strings of commands to run, pipes one into the other
int piping(char *cmd1, char *cmd2);

// takes parsed input, recreates space-separated string
char* parsedtostr(char **parsed);

// takes arguments, executes commands
void eval(char **parsed);

// handle ctrl+c
void INThandler(int sig);

// terminal colors
void boldgreen();
void green();
void white();
