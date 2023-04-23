#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main (int argc, char *argv[])
{
  if ( argc < 2 ) {
    return 1;
  }
  pid_t *pid_array = NULL;
  int pid_count = 0;
  if (argc == 2 )
  {
    //just one cmd, can just go dirtectly to console, no pipe

  }
  //create ID here
  for ( int i = 1; i < argc; i++ ) {
    int fd[2];
    if ( pipe(fd) == -1 ) { return 1; }
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
  waitpid(pid_array[0],NULL,0);
  // Print all child process PIDs
  printf("Child process PIDs:\n");
  for (int i = 0; i < pid_count; i++) {
      printf("%d\n", pid_array[i]);
  }

  // Free memory
  free(pid_array);
}
