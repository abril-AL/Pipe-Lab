#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int main (int argc, char *argv[])
{
  if ( argc < 2 ) {
    return 0;
  }
  if ( argc == 2 ){//only 1 cmd, no need for pipes
    pid_t cid = fork();
    if ( cid == 0 ){
      if (execlp(argv[1], argv[1], (char *) NULL) == -1) {
        return errno;
      }
    }
    wait(NULL);//TODO: add error handling
    return 0;
  }

  int i;

    for( i=1; i<argc-1; i++)
    {
        int pd[2];
        pipe(pd);

        if (!fork()) {
            dup2(pd[1], 1); // remap output back to parent
            execlp(argv[i], argv[i], NULL);
            perror("exec");
            abort();
        }

        // remap output from previous child to input
        dup2(pd[0], 0);
        close(pd[1]);
    }

    execlp(argv[i], argv[i], NULL);
    perror("exec");
    abort();

}