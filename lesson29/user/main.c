#include "my_stdio.h"
#include "my_string.h"
#include <stdio.h>

int main()
{
  My_FILE *fp=Myfopen("log.txt","w");
  Myclose(fp);

  const char *msg="hello world";
  printf("msg len:%d\n",my_strlen(msg));

  return 0;
}
