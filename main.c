#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <readline/readline.h> //sudo apt-get install libreadline-dev
#include <readline/history.h> //gcc -o shell main.c -lreadline

void parse_command(char *cmd);
void execute_command(char *args[]);
void signal_handler(int sig);

int main(){
  signal(SIGINT, signal_handler);
  char *input;
  while(1){
    input = readline("yo> "); //Python Input and Auto Malloc
    if (strcmp(input,"exit")==0){
      free(input);
      break;
    }
    parse_command(input);
    free(input);
  }
  return 0;
}

void execute_command(char *args[]){
  pid_t pid = fork();
  int status;
  if (pid < 0) {
    printf("Fork Failed");
  } else if (pid == 0) {
      signal(SIGINT, SIG_DFL); // Child process always SIGNALINT reset kore nibe
      
      if (strcmp(args[0], "cd") == 0) { // cd command kn jani kaj kore na so handling aladha kore
        if (args[1] == NULL) {
            printf("cd: missing argument\n");
        } else {
            if (chdir(args[1]) != 0) {
                printf("cd execution failed!\n");
            }
        }
        exit(1);
      }

      else if (execvp(args[0], args) < 0) { // cd bade everyother command
          printf("Command execution failed!\n");
          exit(1);
      }
  } else {
    wait(&status);
  }
}

void signal_handler(int sig) { // Shell/Parent CTRL+C Signal ignore korbe
  printf("\nyo> ");
}

void parse_command(char *cmd) {
  char *args[1024];
  char *split = strtok(cmd, " ");
  int i = 0;
  while (split != NULL) {
    args[i++] = split;
    split = strtok(NULL, " ");
  }
  args[i] = NULL;
  execute_command(args);
}