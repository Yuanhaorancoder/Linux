#include"my_stdio.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
static mode_t gmode=0666;

//My_FILE *fp=Myopen("log.txt","a");
My_FILE *Myfopen(const char *pathname,const char *mode)// r w a
{
  if(pathname==NULL || mode==NULL) return NULL;
  umask(0);
  int fd=0;
  int flags=0;
  if(strcmp(mode,"w")==0)
  {
    flags=O_CREAT|O_WRONLY|O_TRUNC;
    fd=open(pathname,flags,gmode);
    (void)fd;
  }  
  if(strcmp(mode,"r")==0)
  {
    flags=O_RDONLY; 
    fd=open(pathname,flags);
    (void)fd;
  }  
  if(strcmp(mode,"a")==0)
  {
    flags=O_CREAT|O_WRONLY|O_APPEND;
    fd=open(pathname,flags,gmode);
    (void)fd;
  }  
  else{}

  if(fd<0)
    return NULL;
  // 创建My_FILE对象
  My_FILE *fp=(My_FILE*)malloc(sizeof(My_FILE));
  if(!fp)
    return NULL;
  
  fp->fd=fd;
  fp->flags=flags;
  fp->mode =LINE_CACHE;
  fp->cap=1024;
  fp->size=0;
  fp->outbuffer[0]='\0';
  memset(fp->outbuffer,0,sizeof(fp->outbuffer));
  return fp;

}

int Myfwrite(const char *message,int size,int num,My_FILE *fp)
{
  if(message==NULL||fp==NULL) 
    return -1;
  // 向文件里面写，本质是向缓冲区写
  int total=size*num;
  if(total+fp->size > fp->cap-1) 
    return -1;

  //写入
  memcpy(fp->outbuffer+fp->size ,message,total);
  fp->size+=total;
  fp->outbuffer[fp->size]=0;

  if(fp->outbuffer[fp->size-1]=='\n' && (fp->mode & LINE_CACHE))
    Myfflush(fp);
}

void Myfflush(My_FILE *fp)
{
  if(!fp)
    return;
  //判断是否刷新
  if(fp->size>0)
  {
    // 系统调用
    // 从用户缓冲区拷贝到内核,WB, Write Back
    write(fp->fd,fp->outbuffer,fp->size);
    fp->size=0;
    // WT—Write Through
    // 不仅仅要写入到内核缓冲区，必须给我写到对应的硬件上
    fsync(fp->fd);
  }
}

void Myclose(My_FILE *fp)
{
  if(!fp)
    return;
  // 先刷新再关闭
  Myfflush(fp);
  close(fp->fd);
}
