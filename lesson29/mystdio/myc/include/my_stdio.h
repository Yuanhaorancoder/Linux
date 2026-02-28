#pragma once

typedef struct
{
  int fd;
  int flags;
  int mode;// 刷新策略
  char outbuffer[1024];
  int cap;
  int size;
  //char inbuffer[1024];
}My_FILE;

#define NONE_CACHE 1 
#define LINE_CACHE 2
#define FULL_CACHE 4

My_FILE *Myfopen(const char *pathname,const char *mode);// r w a
int Myfwrite(const char *message,int size,int num,My_FILE *fp);
void Myfflush(My_FILE *fp);
void Myclose(My_FILE *fp);
