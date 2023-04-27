#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
//TODO: add error handling(???)

int main (int argc, char *argv[])
{
  int status = 0;
  if ( argc == 1 ) {
    return 0;
  }
  if ( argc == 2 ){//only 1 cmd, no need for pipes
    pid_t cid = fork();
    if ( cid == 0 ){
      if (execlp(argv[1], argv[1], (char *) NULL) == -1) {
        printf("error: call to %s failed\n",argv[1]);
        return errno;
      }
    }
    wait(&status);
    return 0;
  }

  //more than 1 cmd ( need pipes )
  int fd[2];
  int i;
  for ( i = 1; i < argc-1; i++ ) {
    if ( pipe(fd) == -1 ) { return 1; }//[0] read [1] write
    pid_t cid = fork();
    if ( cid == 0 ){ // child call execlp
      dup2(fd[1],1);
      close(fd[0]);
      int err = execlp(argv[i], argv[i], (char *) NULL);
      if (err == -1) { printf("error: call to %s failed\n",argv[i]); return errno; };
    }
    dup2(fd[0], 0);//prev child output into input
    close(fd[1]);//close write
  }
  //run last cmd
  for ( int i = 0; i < argc-1; i++){
    wait(&status);
  }
  int err = execlp(argv[i], argv[i], (char *) NULL);   
  if (err == -1) { printf("error: call to %s failed\n",argv[i]); return errno; };
  return 1;
}