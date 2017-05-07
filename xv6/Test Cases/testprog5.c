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
		  "Test Program 5: Create 2 small files in a new small directory, one tries to exceed maximum byte limit\n$\n");

  // execute mkSmallFilesdir command, create a small directory smallDir
  printf(1, "$ mkSmallFilesdir smallDir\n");
  mkSmallFilesdir("smallDir");

  // execute mksmf command, create 2 small files in smallDir, the first one has
  // 52 bytes and the second one has 53 bytes, which exceeds the maximum byte limit
  // An error will occur when creating the second file as it has too many bytes
  printf(1, "$ mksmf smallDir/small_52bytes.txt ThisTestCaseWillTryToReachTheMaximumByteLimit52Bytes\n");
  int fd = open("smallDir/small_52bytes.txt", O_CREATE|O_SMALLFILE|O_RDWR);
  char str1[52] = "ThisTestCaseWillTryToReachTheMaximumByteLimit52Bytes";
  write(fd, str1, strlen(str1));
  close(fd);

  printf(1, "$ mksmf smallDir/small_53bytes.txt ThisTestCaseWillTryToExceedTheMaximumByteLimit52Bytes\n");
  fd = open("smallDir/small_53bytes.txt", O_CREATE|O_SMALLFILE|O_RDWR);
  char str2[53] = "ThisTestCaseWillTryToExceedTheMaximumByteLimit52Bytes";
  write(fd, str2, strlen(str2));
  close(fd);

  // execute ls command, display the content of smallDir, small_53bytes.txt should not exist
  printf(1, "$ ls smallDir\n");
  ls("smallDir");

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
