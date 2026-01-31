#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main()
{

  int fda=open("loga.txt",O_RDONLY);
  dup2(fda,0);
  int a=0;
  float f=0.0;
  char c=0;
  scanf("%d %f %c",&a,&f,&c);
  printf("%d,%f,%c\n",a,f,c);

  // int fda=open("loga.txt",O_CREAT | O_WRONLY | O_APPEND,0666);
 // dup2(fda,1);
 // printf("hello printf\n"); // stdout -> 1
 // fprintf(stdout, "hello fprintf\n"); // stdout -> 1
  
  //int fda=open("loga.txt",O_CREAT | O_WRONLY | O_TRUNC,0666);
  //dup2(fda,1);
  //printf("hello printf\n"); // stdout -> 1
  //fprintf(stdout, "hello fprintf\n"); // stdout -> 1

  //printf("stdin:%d \n",stdin->_fileno);
  //printf("stdout:%d\n",stdout->_fileno);
  //printf("stderr:%d\n",stderr->_fileno);

  //FILE *fp=fopen("log.txt","w");
  //printf("log.txt:%d\n",fp->_fileno);// 3

  //fprintf(stdout,"hello stdout\n");
  //close(0);
  //int fda=open("loga.txt",O_CREAT | O_WRONLY | O_TRUNC,0666);
  //int fdb=open("logb.txt",O_CREAT | O_WRONLY | O_TRUNC,0666);
  //int fdc=open("logc.txt",O_CREAT | O_WRONLY | O_TRUNC,0666);
  
  //fprintf(stdout,"fda:%d\n",fda);
  //fprintf(stdout,"fdb:%d\n",fdb);
  //fprintf(stdout,"fdc:%d\n",fdc);
  //printf("fda:%d\n",fda);
  //printf("fdb:%d\n",fdb);
  //printf("fdc:%d\n",fdc);
 
  //close(fda);
  //close(fdb);
  //close(fdc);
  return 0;
}

























//int main(int argc,char *argv[])
//{
//  if(argc!=2)
//  {
//    printf("Usage: %s filename\n",argv[0]); // ./myfile filename
//    return 1;
//  }
//  int fd=open(argv[1],O_RDONLY);
//  if(fd<0)
//  {
//    perror("open");
//    return 2;
//  }
//
//  char inbuffer[128];
//  while(1)
//  {
//    ssize_t n=read(fd,inbuffer,sizeof(inbuffer)-1);
//    if(n>0)
//    {
//      inbuffer[n]=0;
//      printf("%s",inbuffer); 
//    }
//    else if(n==0)
//    {
//      printf("end of file!\n");
//      break;
//    }
//    else 
//    {
//      perror("read");
//      break;
//    }
//  }
//
//  close(fd);
//  return 0;
//}








//int main()
//{
//  umask(0);
//  int fd=open("log.txt",O_CREAT | O_WRONLY | O_APPEND,0666);
//  if(fd<0)
//  {
//    perror("open");
//    return 1;
//  }
//
//  const char *message="hello bit\n";
//  int cnt=10;
//  while(cnt--)
//  {
//    write(fd,message,strlen(message));
//  }
//
//  close(fd);
//  return 0;
//}






//int main()
//{
//  umask(0);
//  int fd=open("log.txt",O_CREAT | O_WRONLY | O_TRUNC,0666);
//  if(fd<0)
//  {
//    perror("open");
//    return 1;
//  }
//
//  //const char *message="1234567890abcdefg\n";
//  const char *message="ccc";
//  //write(fd,message,strlen(message));
//
//  close(fd);
//  return 0;
//}







//#define ONE (1<<0)   // 1
//#define TWO (1<<1)   // 2
//#define THREE (1<<2) // 4
//#define FOUR (1<<3)  // 8 
//#define FIVE (1<<4)  // 16
//
//void Print(int flags)
//{
//  if(flags & ONE)
//    printf("ONE\n");
//  if(flags & TWO)
//    printf("TWO\n");
//  if(flags & THREE)
//    printf("THREE\n");
//  if(flags & FOUR)
//    printf("FOUR\n");
//  if(flags & FIVE)
//    printf("FIVE\n");
//}
//
//int main()
//{
//
//  Print(ONE);
//  printf("\n");
//  Print(TWO);
//  printf("\n");
//  Print(ONE | TWO);
//  printf("\n");
//  Print(ONE | TWO | THREE);
//  printf("\n");
//  Print(ONE | TWO | THREE | FOUR);
//  printf("\n");
//  Print(TWO | THREE | FOUR | FIVE);
//
//  return 0;
//}








//int main()
//{
//  //chdir("/home/yhr");
//
//  //char pwd[64];
//  //getcwd(pwd,sizeof(pwd));
//  //printf("cwd:%s\n",pwd);
//
//  FILE *fp=fopen("log.txt","r");
//  if(fp==NULL)
//  {
//    perror("fopen");
//    return 0;
//  }
//
//  char inbuffer[1024];
//  while(1)
//  {
//    long pos=ftell(fp);
//    printf("pos:%ld\n",pos);
//    int ch=fgetc(fp);
//    if(ch==EOF)
//    {
//      break;
//    }
//    printf("%c\n",ch);
//    
//    //if(!fgets(inbuffer,sizeof(inbuffer),fp))
//    //{
//    //  break;
//    //}
//      printf("file:%s",inbuffer);
//  }
//
//  //const char *message="abcd\n";
//  //fputs(message,fp);
//  //while(cnt<10)
//  //{
//
//  //  //fwrite(message,1,strlen(message),fp);
//  //  fputs(message,fp);
//  //  //fprintf(fp,"hello bit:%d\n",cnt);
//  //  cnt++;
//  //}
//
//  fclose(fp);
//  
//  return 0;
//}
