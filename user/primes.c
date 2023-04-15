#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, int *argv[])
{
  // int pl[2],pr[2];
  // pipe(pl);
  // pipe(pr);

  // int pid =fork();
  // // if (pid>0) {

  // // }
  // // else if (pid==0) {

  // // }
  // while (fork()==0) {
  //   int p;
  //   read(pl[0],&p,sizeof(int));
  //   printf("prime %d",p-'0');
  //   int num;
  //   while (read(pl[0],&num,sizeof(int))>0)
  //   {
  //     if ()
  //     write(pr[1],&num,sizeof(int));
  //   }
  // }

  int p[2],end=0;
  pipe(p);
  for (int i = 2; i <= 35; i++)
    write(p[1], &i, sizeof(int));
  write(p[1], &end, sizeof(int));

  while (fork() == 0)
  {
    int pnum;
    
    if (read(p[0], &pnum, sizeof(int)) > 0 && pnum==0)
      break;

    printf("prime %d\n", pnum);
    int n, nums[34] = {0};
    for (int i = 0; read(p[0], &n, sizeof(int)) > 0 && n>0; i++)
      nums[i] = n;

    for (int i = 0; i < 34 && nums[i] > 0; i++)
    {
      if (nums[i] % pnum != 0)
      {
        write(p[1], &nums[i], sizeof(int));
      }
    }
    write(p[1], &end, sizeof(int));
  }
  wait((int *)0);
  exit(0);
}