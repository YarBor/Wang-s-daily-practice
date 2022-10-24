#include<stdio.h>
#define MAX1(x, y) x > y ? x :y
#define MAX2(x, y) ((x) > (y) ? (x) : (y))
#define YEAR 2026
#define Y(x) YEAR* x* x

// int main() {
//     printf("%d\n", MAX1(1, 2));
//     printf("%d\n", 100 + MAX2(1 != 1, 1 != 2));

int main(int argc, char **argv) {
  int data1[2][3] = {{0X636c6557, 0X20656d6f, 0X58206f74},
                     {0X756f7969, 0X6e694c20, 0x00000000}};
  int data2[] = {0X47207875, 0X70756f72, 0X32303220, 0X00000a32};

  char *a = (char *)data1;
  char *b = (char *)data2;
  char buf[1024];
  strcpy(buf, a);
  strcat(buf, b);
  printf("%s\n", buf);
}