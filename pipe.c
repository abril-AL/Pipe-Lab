#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main (int argc, char *argv[])
{
  if ( argc < 2 ) {
    exit(EXIT_FAILURE);
  }
  if ( argc == 2 ){//only 1 cmd, no need for pipes
    pid_t cid = fork();
    if ( cid == 0 ){
      if (execlp(argv[1], argv[1], (char *) NULL) == -1) {
        return errno;
      }
    }
  }
  pid_t *pid_array = NULL;
  int pid_count = 0;
  


  for ( int i = 1; i < argc; i++ ) {
    int fd[2];
    if ( pipe(fd) == -1 ) { return 1; }//[0] read [1] write
    if( i==1 ){//first child - input from parent’s stdin
         
    }
    else if (i != argc-1){//last child - outputs to parent’s stdout

    }
    else{//child in between

    }

    pid_t cid = fork();
    if ( cid == 0 ){
      if (execlp(argv[i], argv[i], (char *) NULL) == -1) {
        return errno;
      }
    }
    else{
      pid_count++;
      pid_array = realloc(pid_array, pid_count * sizeof(pid_t));
      pid_array[pid_count-1] = cid;
    }
  }






  //wait for all processes
  for (int i = 0; i < argc-1; i++) {
    int status;
    pid_t result = waitpid(pid_array[i], &status, 0);

    if (result == -1) {
      printf("Error waiting for child process %d\n", i+1);
    } else if (WIFEXITED(status)) {
      //printf("Child process %d with PID %d exited with status %d\n", i+1, pid_array[i], WEXITSTATUS(status));
    }
  }
  // Print all child process PIDs  -  printf("Child process PIDs:\n");
  for (int i = 0; i < pid_count; i++) {
    //printf("%d\n", pid_array[i]);
  }
  // Free memory
  free(pid_array);
}
