#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  int fd = open("time.txt", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf("time: cannot open time.txt\n");
    exit(1);
  }
  write(fd, "time test\n", 10);
  close(fd);
  exit(0);
}
