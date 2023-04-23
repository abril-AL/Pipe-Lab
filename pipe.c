#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main (int argc, char *argv[])
{
  if ( argc < 2 ) {
    return 1;
  }
  for ( int i = 1; i < argc; i++ ) {
    int fd[2];
    if ( pipe(fd) == -1 ) { return 1; }
    pid_t cid = fork();
    if ( cid == 0 ){
      char cmd[] = argv[i];
      char * cArg = {NULL};
      char * const cEnvr = { "PATH=/bin", "USER=me", NULL };
      printf(cmd,cArg,cEnvr);
      execvp(cmd,cEnvr);
    }
    waitpid(cid,NULL,0);
  }
}
