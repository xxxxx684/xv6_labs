#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buf[64];
  char *xargs[32];

  /* 把初始命令参数拷进 xargs[0..argc-2] */
  for (int i = 1; i < argc; ++i)
    xargs[i - 1] = argv[i];

  /* 主循环：逐行读取 stdin，按空格分词，追加到 argv 尾部，fork+exec */
  while (1) {
    int x = argc - 1;          /* 当前参数个数 */

    gets(buf, 64);
    if (buf[0] == 0)           /* EOF 或空行 -> 结束 */
      break;

    /* 把整行追加到参数列表 */
    xargs[x++] = buf;

    /* 按空格切分，把空格换成 '\0' 并把后面单词指针填进 xargs */
    for (char *p = buf; *p; ++p) {
      if (*p == ' ') {
        *p = 0;
        xargs[x++] = p + 1;
      } else if (*p == '\n') {
        *p = 0;
      }
    }

    if (fork() == 0) {
      exec(argv[1], xargs);    /* 子进程执行命令 */
    }
  }

  wait(0);                     /* 父进程等最后一个子进程结束 */
  exit(0);
}
