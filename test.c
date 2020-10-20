/*一个测试程序*/
#include "base64.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("invaild parameter \n");
    return -1;
  }
  // unsigned char *buf =NULL;
  unsigned char buf[1024] = {0};
  if (strcmp(argv[1], "-d") == 0) {
    if (argc != 3) {
      printf("invaild parameter \n");
      return -1;
    }
    int ret = base64_decode(argv[2], buf, sizeof(buf));
    if (ret > 0) {
      printf("decode len: %d and decode string:%s\n", ret, buf);
    }
  } else {
    if (argc != 2) {
      printf("invaild parameter \n");
      return -1;
    }

    int ret = base64_encode(argv[1], buf, sizeof(buf));
    if (ret > 0) {
      printf("encode len: %d and encode string:%s\n", ret, buf);
    }
  }

  //    free(buf);

  return 0;
}
