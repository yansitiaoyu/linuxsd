#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
/*
 *创建一个用于命名管道的读进程
 */

int main(int argc,char *argv[])
{
	if(argc<2){
		printf("usage:%s fifo\n",argv[0]);
		exit(1);
	}
	/*if(mkfifo(argv[1],O_RDONLY)<0){
		perror("mkfifo error");
		exit(1);
	}*/
	int fd=open(argv[1],O_RDONLY);
	char buff[1024];
	memset(buff,0,sizeof(buff));
	if(read(fd,buff,sizeof(buff))<0){
		perror("read error");
		exit(1);
	}else{
		printf("read fifo success:%d\n",fd);
	}
	close(fd);
	printf("%s\n",buff);
	exit(0);
}
