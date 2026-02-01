#pragma once

typedef struct
{
  int fd;
  int flags;
  int mode;// 刷新策略
  char outbuffer[1024];
  //char inbuffer[1024];
}My_FILE;

My_FILE *Myopen(const char *pathname,const char *mode);// r w a
