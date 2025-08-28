#include "kernel/types.h"
#include "user/user.h"

void
prime_filter(int p0[2])
{
  close(p0[1]);
  int prime;
  if (read(p0[0], &prime, sizeof(int)) == 0) {
    close(p0[0]);
    exit(0);
  }

  printf("prime %d\n", prime);

  int p1[2];
  pipe(p1);

  if (fork() == 0) {
    /* 子进程：继续处理下一个素数 */
    close(p0[0]);            /* 不再读取左侧管道 */
    prime_filter(p1);
  } 
else {
    /* 父进程：过滤并传递剩余数字 */
    int num;
    while (read(p0[0], &num, sizeof(int))) {
      if (num % prime != 0) {
        write(p1[1], &num, sizeof(int));
      }
    }
    close(p0[0]);
    close(p1[1]);            /* 通知子进程 EOF */
    wait(0);
    exit(0);
  }
}

int
main(void)
{
  int p[2];
  pipe(p);

  if (fork() == 0) {
    /* 子进程：从管道读取并筛选 */
    prime_filter(p);
  } else {
    /* 父进程：生成 2~35 并写入管道 */
    close(p[0]);
    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]);             /* 通知子进程 EOF */
    wait(0);
    exit(0);
  }
}
