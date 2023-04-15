#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pp[2];
  int pc[2];
  pipe(pp);
  pipe(pc);
  int _pid=fork();
  if (_pid > 0) {
    char buf;
    write(pp[1],"x",1);
    wait((int *) 0);
    if (read(pc[0],&buf,1)==1) {
      printf("%d: received pong\n",getpid());
    }
  }
  else if (_pid==0) {
    char buf;
    if (read(pp[0],&buf,1)==1) {
      printf("%d: received ping\n",getpid());
    }
    write(pc[1],"x",1);
  }
  exit(0);
}
