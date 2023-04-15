#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h" // 必须以这个顺序 include，由于三个头文件有依赖关系
#include "kernel/fs.h"

char *
fmtname(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // find_recur first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)]='\0';
  // memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}

void find_recur(char *path, char *dis)
{
  char buf[512], *p;
  char *name;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch (st.type)
  {
  case T_FILE:
    
    name = fmtname(path);
    if (strcmp(name, dis) == 0)
    {
      printf("%s\n", path);
    }
    break;

  case T_DIR:
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
      printf("find_recur: path too long\n");
      break;
    }
    if (strcmp(fmtname(path), dis) == 0)
    {
      printf("%s\n", path);
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
      if (de.inum == 0 || strcmp(de.name,".")==0 || strcmp(de.name,"..")==0)
        continue;
      memmove(p, de.name, strlen(de.name));
      p[strlen(de.name)] = 0;
      if (stat(buf, &st) < 0)
      {
        printf("find_recur: cannot stat %s\n", buf);
        continue;
      }
      find_recur(buf,dis);
    }
    break;
  }
  close(fd);
}

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    printf("usage: find\n");
  }

  char *src = argv[1], *dis = argv[2];

  find_recur(src,dis);

  exit(0);
}