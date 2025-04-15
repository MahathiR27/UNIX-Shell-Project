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

void execute_command(char *cmd);

int main(){
  char *input;
  while(1){
    input = readline("yo> "); //Python Input and Auto Malloc
    if (strcmp(input,"exit")==0){
      free(input);
      break;
    }
    execute_command(input);
    free(input);
  }
  return 0;
}

void execute_command(char *cmd){
  char *args[100];
  char *split = strtok(cmd, " "); //.split() the input(Parsing technically)
  int i = 0;
  while (split != NULL){
    args[i++] = split; 
    // printf("%s \n", args[i - 1]);
    split = strtok(NULL, " ");//Split kora input ke ek space kore agay
  }
  args[i] = NULL; // Apearently last element NULL na hoile execvp kaj kore na

  pid_t pid = fork();
  int status;
  if (pid < 0) {
    printf("Fork Failed");
  } else if (pid == 0) {
      signal(SIGINT, SIG_DFL); // Reset SIGINT to default behavior for the child

      if (execvp(args[0], args) < 0) { // Execute the command
          printf("Command execution failed!\n");
          exit(1);
      }
  } else {
    wait(&status);
  }
}