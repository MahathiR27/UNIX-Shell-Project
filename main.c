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

void redirection_command(char *cmd);
void pipe_command(char *cmd);
void multiple_command(char *cmd);
void parse_command(char *cmd);
void execute_command(char *args[]);
void signal_handler(int sig);

int main(){
  signal(SIGINT, signal_handler);
  using_history(); // readline library function jeita diye history storeage create kore.
  // Arrow key diye history anar jonne basically
  char *input;
  while(1){
    input = readline("yo> "); //Python Input and Auto Malloc
    if (input == NULL){
      continue;
    }
    else if (strcmp(input,"exit")==0){
      free(input);
      break;
    }
    else{
      add_history(input); // RAW Input ta ke just oi history storage e add kore dey
    }
    if (strchr(input, ';') != NULL) {
      multiple_command(input); // ; ala commands der split kore loop kore parse e pathabe
    }
    else if (strchr(input, '|') != NULL) {
      pipe_command(input);
    } 
    else if (strchr(input, '<') != NULL || strchr(input, '>') != NULL) {
      redirection_command(input);
    }
    else{
    parse_command(input);
    }
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
  printf("yo> ");
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

void multiple_command(char *cmd) {
  char *commands[1024]; // New array jeitay ; r sob commands store korbo
  int count = 0;

  char *command = strtok(cmd, ";");
  while (command != NULL) {
    commands[count++] = command;
    command = strtok(NULL, ";");
  }

  for (int i = 0; i < count; i++) { // Loop kore sob gula commands ek ta ekta parse kore execute korte dibo
    parse_command(commands[i]);
  }
}

void pipe_command(char *cmd) {
  char *commands[1024];
  int count = 0;

  // Split the input by '|'
  char *command = strtok(cmd, "|");
  while (command != NULL) {
    commands[count++] = command;
    command = strtok(NULL, "|");
  }

  int pipefd[2];
  int fd_in = 0; // Basically commands er input outpoint kontha theke nibe point korar jnne

  for (int i = 0; i < count; i++) {
    pipe(pipefd); 

    pid_t pid = fork();
    int status;
    if (pid == 0) {
      dup2(fd_in, 0); // Standard input terminal theke na niye file theke nibe
      if (i < count - 1) {
        dup2(pipefd[1], 1); // Same for output
      }
      close(pipefd[0]); 
      parse_command(commands[i]); // Execute the command
      exit(0);
    } else {
      wait(&status);
      close(pipefd[1]); 
      fd_in = pipefd[0]; // last output ta jate next command read kore tai fd_in e set kore dibe
    }
  }
}

void redirection_command(char *cmd) {
  char *args[1024];
  char *input_file = NULL;
  char *output_file = NULL;
  int append = 0; 

  char *split = strtok(cmd, " "); 
  int i = 0;
  while (split != NULL) { // Space diye split korar pore 2 tar mazjkhane asole kon oparator ase ber kore
    if (strcmp(split, "<") == 0) { // O_RDONLY
      split = strtok(NULL, " ");
      if (split != NULL) {
        input_file = split;
      }
    } else if (strcmp(split, ">") == 0) { // O_WRONLY
      split = strtok(NULL, " ");
      if (split != NULL) {
        output_file = split;
        append = 0;
      }
    } else if (strcmp(split, ">>") == 0) { // O_APPEND
      split = strtok(NULL, " ");
      if (split != NULL) {
        output_file = split;
        append = 1; 
      }
    } else {

      args[i++] = split; // Args e just split kore add dise
    }
    split = strtok(NULL, " ");
  }
  args[i] = NULL; 

  pid_t pid = fork();
  int status;
  if (pid == 0) {
    if (input_file != NULL) {
      int fd_in = open(input_file, O_RDONLY);

      dup2(fd_in, 0); // Terminal theke input na niye file theke niye store kore
      close(fd_in);
    }

    else if (output_file != NULL) {
      int fd_out;
      if (append) {
        fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
      } else {
        fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); // O_TRUNC =  Truncation file
      }

      dup2(fd_out, 1); // Terminal e na diye file e output
      close(fd_out);
    }

    execvp(args[0], args);

    exit(1);
  } else if (pid > 0) {
    wait(&status);
}
}