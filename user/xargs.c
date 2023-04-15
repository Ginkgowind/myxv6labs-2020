#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h" // 必须以这个顺序 include，由于三个头文件有依赖关系
#include "kernel/param.h"

#define MAXCHARNUM MAXARG

int main(int argc, char **argv)
{
  char *myargv[MAXARG];
  for (int i = 1; i < argc; i++)
    myargv[i - 1] = argv[i];
  int index = argc - 1;
  // 开始读取每一行
  char *buf = malloc(MAXCHARNUM * sizeof(char));
  char *p = buf;
  char ch;
  while (read(0, &ch, sizeof(char)) > 0)
  {
    if (ch == '\n')   // 做完才想起来忘了处理空格，这次就算了
    {
      myargv[index++] = buf;
      myargv[index] = 0;
      if (fork() == 0)
      {
        exec(myargv[0], myargv);
      }
      wait((int *)0);
      buf = malloc(MAXCHARNUM * sizeof(char));
      p = buf;
      index = argc - 1;
      // TODO:这里还要加上对之前malloc空间的delete，
      // 或者设置一个2048的buffer从头开始往后推进，使用p和last_p来标识其中的每一个单词，再一个个地赋值给myargv
    }
    else
    {
      *p++ = ch;
    }
  }
  if (index > argc - 1) // 判断是不是空行
  {
    myargv[index++] = buf;
    if (fork() == 0)
    {
      exec(myargv[0], myargv);
    }
    wait((int *)0);
  }

  exit(0);
}