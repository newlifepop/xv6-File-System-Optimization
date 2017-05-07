#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

char *fmtname(char *path);
void ls(char *path);

int main(void)
{
  printf(1, "CS450 PA4, Group 27\nAndrew Patterson and Changyu Wu\n"
		  "Test Program 2: Create a regular file and a small file in a new small directory\n$\n");

  // execute mkSmallFilesdir command, create a small directory smallDir
  printf(1, "$ mkSmallFilesdir smallDir\n");
  mkSmallFilesdir("smallDir");

  // execute mksmf command, create a small file "small.txt" under smallDir directory and
  // write "hello" to it
  printf(1, "$ mksmf smallDir/small.txt hello\n");
  int fd = open("smallDir/small.txt", O_CREATE|O_RDWR|O_SMALLFILE);
  write(fd, "hello", strlen("hello"));
  close(fd);

  // execute echo command, create a regular file regular.txt under smallDir directory and
  // write "hello" to it. An error will occur as smallDir is not a regular directory
  printf(1, "$ echo hello > smallDir/regular.txt\n");
  fd = open("smallDir/regular.txt", O_CREATE|O_RDWR);
  write(fd, "hello", strlen("hello"));
  close(fd);

  // execute ls command, display the content of smallDir, regular.txt file should not exist
  printf(1, "$ ls smallDir\n");
  ls("smallDir");

  // execute cat command, read from small.txt file, "hello" should be printed
  printf(1, "$ cat smallDir/small.txt\n");
  fd = open("smallDir/small.txt", 0);
  char buf[512];
  int n = read(fd, buf, sizeof(buf));
  write(1, buf, n);
  printf(1, "\n");

  exit();
}

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
  case T_SMALLFILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
  case T_SMALLDIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}
