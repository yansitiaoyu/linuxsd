#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 *创建一个对命名管道fifo的写进程
 */

int main(int argc,char* argv[])
{
	if(argc<0){
		printf("usage:%s fifo\n",argv[0]);
		exit(0);
	}
	/*if(mkfifo(argv[1],O_WRONLY)<0){
		perror("mkfifo error");
		exit(1);
	}*/
	int fd=open(argv[1],O_WRONLY);
	char buff[]="12345678910";
	//memset(buff,0,sizeof(buff));
	if(write(fd,buff,sizeof(buff))!=sizeof(buff)){
		printf("write error");
		exit(1);
	}else{
		printf("fifo write success:%d\n",fd);
	}
	close(fd);
	return 0;
}
