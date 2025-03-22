#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

//父子进程同时操作一个文件，父进程修改文件偏移量

int main(int argc,char *argv[])
{
	if(argc<2){
		fprintf(stderr,"usage: %s file\n",argv[0]);
		exit(1);
	}
	int fd_father=open(argv[1],O_WRONLY | O_CREAT | O_TRUNC,0777);
	char *buff="hello fork!";
	//ssize_t size=sizeof(buff);
	ssize_t size=strlen(buff);
	if(fd_father<0){
		perror("open error:");
	}
	else{
		printf("father process open file seccess\n");
	}
	/*
	   if((size=read(fd,buff,sizeof(buff)))<0){
		perror("read error:");
		exit(1);
	}
	*/
	if(write(fd_father,buff,size)!=size){
		perror("write error:");
		exit(1);
	}
	else{
		printf("father process write success!\n");
	}
	//父进程fork()子进程
	pid_t pid=fork();
	if(pid<0){
		perror("fork error:");
		exit(1);
	}
	else if(pid>0){
		//父进程执行，修改fd文件指针偏移量；
		if(lseek(fd_father,10,SEEK_END)<0){
			perror("lseek error:");
			exit(1);
		}
	}
	else{
		//子进程执行代码，往父进程重置指针的fd文件中写入数据；
		int fd_son=open(argv[1],O_WRONLY | O_APPEND);
	    if(fd_son<0){
		    perror("open error:");
	    }
	    else{
		    printf("son process open file seccess\n");
	    }
		if(write(fd_son,buff,size)!=size){
			perror("son process error:");
			exit(1);
		}
		else{
			printf("son process write success!");
		}
	}
	return 0;
}
