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
  if (argc == 2 )
  {
    //just one cmd, can just go dirtectly to console, no pipe

  }
  for ( int i = 1; i < argc; i++ ) {
    int fd[2];
    if ( pipe(fd) == -1 ) { return 1; }
    pid_t cid = fork();
    if ( cid == 0 ){
      if (execlp("ls", "ls", (char *) NULL) == -1) {
        return errno;
      }
    }
    waitpid(cid,NULL,0);
  }
}
