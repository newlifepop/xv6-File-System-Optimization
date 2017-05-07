#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void)
{
  printf(1, "CS450 PA4, Group 27, Andrew Patterson and Changyu Wu\n"
		  "Test Program 1: Create a regular file and a small file under root directory\n$\n");

  // execute echo command, create a regular file regular.txt under root directory
  // and write "hello" to it
  printf(1, "$ echo hello > regular.txt\n");
  int fd = open("regular.txt", O_CREATE|O_RDWR);
  write(fd, "hello", strlen("hello"));
  close(fd);

  // execute cat command, display the content of regular.txt
  printf(1, "$ cat regular.txt\n");
  char buf[512];
  fd = open("regular.txt", 0);
  int n = read(fd, buf, sizeof(buf));
  write(1, buf, n);
  close(fd);

  // execute mksmf command, create a small file under root directory and write
  // "hello" to it. An error will occur as root directory is not created by mkSmallFilesdir
  printf(1, "\n$ mksmf small.txt hello\n");
  fd = open("small.txt", O_CREATE|O_SMALLFILE|O_RDWR);
  write(fd, "hello", strlen("hello"));
  close(fd);

  exit();
}
