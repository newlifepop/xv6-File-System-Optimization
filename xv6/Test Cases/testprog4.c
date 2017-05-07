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
		  "Test Program 4: Link a regular file to a small file, a small file to a regular file\n"
		  "a regular file to a regular file and a small file to a small file\n$\n");

  // execute mkdir command, create a regular directory regularDir
  printf(1, "$ mkdir regularDir\n");
  mkdir("regularDir");

  // execute mkSmallFilesdir command, create a small directory smallDir
  printf(1, "$ mkSmallFilesdir smallDir\n$\n");
  mkSmallFilesdir("smallDir");

  // execute echo command, create a regular file regular.txt under regularDir and
  // write "hello" to it
  printf(1, "$ echo hello > regularDir/regular.txt\n");
  int fd = open("regularDir/regular.txt", O_CREATE|O_RDWR);
  write(fd, "hello", strlen("hello"));
  close(fd);

  // execute mksmf command, create a small file small.txt under smallDir and
  // write "hello" to it
  printf(1, "$ mksmf smallDir/small.txt hello\n$\n");
  fd = open("smallDir/small.txt", O_CREATE|O_SMALLFILE|O_RDWR);
  write(fd, "hello", strlen("hello"));
  close(fd);

  // execute ln command, try to link a regular file foo under regularDir to small.txt
  // An error will occur as foo is a regular file and small.txt is a small file
  printf(1, "$ ln smallDir/small.txt regularDir/foo\n");
  link("smallDir/small.txt", "regularDir/foo");

  // execute ln command, try to link a small file foo under smallDir to regular.txt
  // An error will occur as foo is a small file and regular.txt is a regular file
  printf(1, "$\n$ ln regularDir/regular.txt smallDir/foo\n");
  link("regularDir/regular.txt", "smallDir/foo");

  // execute ln command, try to link a regular file foo under regularDir to regular.txt
  printf(1, "$\n$ ln regularDir/regular.txt regularDir/foo\n");
  link("regularDir/regular.txt", "regularDir/foo");

  // execute cat command, display the content of foo, should print "hello", which is the
  // content of regular.txt
  printf(1, "$ cat regularDir/foo\n");
  char buf[512];
  fd = open("regularDir/foo", 0);
  int n = read(fd, buf, sizeof(buf));
  write(1, buf, n);
  close(fd);

  // execute ln command, try to link a small file foo under smallDir to small.txt
  printf(1, "\n$\n$ ln smallDir/small.txt smallDir/foo\n");
  link("smallDir/small.txt", "smallDir/foo");

  // execute cat command, display the content of foo, should print "hello", which is the
  // content of small.txt
  printf(1, "$ cat smallDir/foo\n");
  fd = open("smallDir/foo", 0);
  n = read(fd, buf, sizeof(buf));
  write(1, buf, n);
  close(fd);
  printf(1, "\n");

  exit();
}
