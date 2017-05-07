#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
  printf(1, "CS450 PA4, Group 27\nAndrew Patterson and Changyu Wu\n"
		  "Test Program 2: Create a regular file and a small file in a new regular directory\n$\n");

  // execute mkdir command, create a regular directory "regularDir"
  printf(1, "$ mkdir regularDir\n");
  mkdir("regularDir");

  // execute echo command, write "hello" to regular.txt file under regularDir directory
  printf(1, "$ echo hello > regularDir/regular.txt\n");
  int fd = open("regularDir/regular.txt", O_CREATE|O_RDWR);
  write(fd, "hello", strlen("hello"));
  close(fd);

  // execute cat command, display the content of regular.txt file
  printf(1, "$ cat regularDir/regular.txt\n");
  fd = open("regularDir/regular.txt", 0);
  char buf[512];
  int n = read(fd, buf, sizeof(buf));
  write(1, buf, n);
  printf(1, "\n");
  close(fd);

  // execute mksmf command, create a small file under regularDir directory and write
  // "hello" to it. An error will occur as regularDir is not created by mkSmallFilesdir
  printf(1, "$ mksmf regularDir/small.txt\n");
  fd = open("regularDir/small.txt", O_CREATE|O_RDWR|O_SMALLFILE);
  write(fd, "hello", strlen("hello"));
  close(fd);

  exit();
}
