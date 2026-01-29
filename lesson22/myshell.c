#include "myshell.h"

#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>

// 提示符相关
static char username[32]; 
static char hostname[64];
static char cwd[256];
static char commandline[256];
// 与命令行相关
static char *argv[64];
static int argc=0;
static const char *sep=" ";//分割符
// 与退出码有关
static int lastcode=0;
// 环境变量相关，应该由bash来维护，从系统配置文件读，直接从系统bash拷贝就可以了
//static char *env[64];
//static int envc=0;

//环境变量相关
char **_environ=NULL;
static int envc=0;

static void GetUserName()
{
  char *_username=getenv("USER");
  strcpy(username,(_username ? _username :"None"));
}

static void GetHostName()
{
  char *_hostname=getenv("HOSTNAME");
  strcpy(hostname,(_hostname ? _hostname :"None"));
}

static void GetCwdName()
{
  //char *_cwd=getenv("PWD");
  //strcpy(cwd,(_cwd ? _cwd :"None"));
  char _cwd[256];
  getcwd(_cwd,sizeof(_cwd));
  if(strcmp(_cwd,"/")==0)
  {
    strcpy(cwd,_cwd);
  }
  else 
  {
    int end=strlen(_cwd)-1;
    while(end>=0)
    {
      if(_cwd[end]=='/')
      {
        // /home/yhr
        strcpy(cwd,&_cwd[end]+1);
        break;
      }
      end--;
    }
  }
}

static void PrintPrompt()
{
  GetUserName();
  GetHostName();
  GetCwdName();
  printf("[%s@%s %s]# ",username,hostname,cwd);
  fflush(stdout);
}

static void GetCommandLine()
{
  if(fgets(commandline,sizeof(commandline),stdin)!=NULL)
  {
    // "abcd\n"->"abcd"
    commandline[strlen(commandline)-1]=0;
    //printf("debug: %s\n",commandline);
  }
}

static void ParseCommandLine()
{
  //清空
  argc=0;
  memset(argv,0,sizeof(argv));
  //判空
  if(strlen(commandline)==0)
    return;
  //解析
  argv[argc]=strtok(commandline,sep);
  while((argv[++argc]=strtok(NULL,sep)));
  
 // printf("argc: %d\n",argc);
 // int i=0;
 // for(;argv[i];i++)
 // {
 //   printf("argv[%d]: %s\n",i,argv[i]);
 // }
}

void Excute()
{
  pid_t id=fork();
  if(id<0)
  {
    perror("fork");
    return;
  }
  else if(id==0)
  {
    //子进程
    //程序替换
    //cat file.c
    //1.判断命令类型:cat 
    //2.获取用户身份，用户名，包括用户id
    //3.获取文件的属性，用户，所属组，Mode
    //4.权限管理
    execvp(argv[0],argv);
    exit(1);
  }
  else
  {
    //父进程
    int status=0;
    pid_t rid=waitpid(id,&status,0);
    (void)rid;
    lastcode=WEXITSTATUS(status);
  }
}

// 1:yes 
// 0:no,普通命令,让子进程执行 
int CheckBuiltinAndExecute()
{
  int ret=0;
  if(strcmp(argv[0],"cd")==0)
  {
    //内建命令
    ret=1;
    // cd 默认有选项，就切换路径
    if(argc==2)
    {
      chdir(argv[1]);
    }
  }
  else if(strcmp(argv[0],"echo")==0)
  {
    ret=1;
    if(argc==2)
    {
      // echo $? 
      // echo $PATH
      // echo "hello world"
      // echo helloworld
      if(argv[1][0]=='$')
      {
        if(strcmp(argv[1],"$?")==0)
        {
          printf("%d\n",lastcode);
          lastcode=0;
        }
        else 
        {
          // env
        }
      }
      else 
      {
        printf("%s\n",argv[1]);
      }
    }
  }
  else if(strcmp(argv[0],"env")==0)
  {
    ret=1;
    int i=0;
    for(;i<envc;i++)
    {
      printf("%s\n",_environ[i]);
    }
  }
  else if(strcmp(argv[0],"export")==0)
  {
    ret=1;
    if(argc==2)
    {
      // export myval=100
      char *mem=(char*)malloc(strlen(argv[1])+1);
      strcpy(mem,argv[1]);
      _environ[envc++]=mem;
      _environ[envc]=NULL;
    }
  }
  return ret;
}

static void LoadEnv()
{
  extern char **environ;
  for(envc=0;environ[envc];envc++)
  {
    _environ[envc]=environ[envc];
  }
  _environ[envc]=NULL;
}

void Bash()
{
// 环境变量相关，应该由bash来维护，从系统配置文件读，直接从系统bash拷贝就可以了
  static char *env[64];
  _environ=env;

  static char *local[64];// a=100，b=200 本地变量
  //第0步：获取环境变量
  LoadEnv();
  while(1)
  {
    //第1步：输出命令行
    PrintPrompt();

    //第2步：等待用户输入，获取用户输入
    //char commandline[256];
    GetCommandLine();
  
    //第3步：解析字符串,"ls -a -l" -> "ls" "-a" "-l"
    ParseCommandLine();
    if(argc==0)
      continue;

    //第4步：有些命令,cd,echo,env,export等命令，不应该让子进程执行
    //父进程bash自己执行,内建命令,bash内部的函数
    if(CheckBuiltinAndExecute())
    {
      continue;
    }

    //第5步：执行命令
    
    Excute();
    
  }
}
