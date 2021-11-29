// entering shell
void enter_shell();

// returns command line
void get_commandline();

// takes string, reads input into the string
void input(char * c);

// takes string, returns parsed input
char** split(char * c);

// takes arguments, executes commands
void eval(char **parsed);

// prints exit shell statement
void INThandler(int sig);

// terminal colors
void boldgreen();
void green();
void white();
