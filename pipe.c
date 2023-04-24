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

  //more than 1 cmd ( need pipes )
  int fd[2];
  int i;
  for ( i = 1; i < argc-1; i++ ) {
    if( i == 1 ){//first child - input from parent’s stdin
      if ( pipe(fd) == -1 ) { return 1; }//[0] read [1] write
      pid_t cid = fork();
      if ( cid == 0 ){ // child call execlp
        close(fd[0]);
        dup2(fd[1],1);
        int err = execlp(argv[i], argv[i], (char *) NULL);   if (err == -1) { return errno; };
      }
      dup2(pd[0], 0);//prev child output into input
      close(fd[1]);//close write
    }
    /* 
     if (i == argc-1){//last child - outputs to 
      //wont use new pipe
      pid_t cid = fork();
      if ( cid == 0 ){ // child call execlp
        dup2(fd[0],0);
        if (execlp(argv[i], argv[i], (char *) NULL) == -1) { return errno; }
      }else{
        close(fd[0]);//close read fd (end)
      }
     }
     
      else{//child in between
      //get read and write fd and set to  stdin and stdout}
    */
    //run last cmd
    int err = execlp(argv[i], argv[i], (char *) NULL);   if (err == -1) { return errno; };
  }

  return 0;
}