#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

/*
 *管道实现父子进程之间的通信 
 */

int main()
{
	int fd[2];
	pid_t pid;
	int global_setnum=66;
	if(pipe(fd)<0){
		perror("pipe error");
		exit(1);
	}
	if((pid=fork())<0){
		perror("fork error");
		exit(1);
	}else if(pid>0){
		int start=0,end=100;
		int local_setnum=80;
		close(fd[0]);
		if(write(fd[1],&start,sizeof(int))!=sizeof(int)){
			perror("write error");
		}
	    if(write(fd[1],&end,sizeof(int))!=sizeof(int)){
			perror("write error");
		}
		printf("global_setnum:%d\n",global_setnum+1);
		printf("local_setnum:%d\n",local_setnum);
		close(fd[1]);
		wait(NULL);
	}else{
		int start,end;
		close(fd[1]);
		if(read(fd[0],&start,sizeof(int))<0){
			perror("read error");
		}
		if(read(fd[0],&end,sizeof(int))<0){
			perror("read error");
		}
		close(fd[0]);
		printf("global_setnum:%d\n",global_setnum);
		//printf("local_setnum:%d\n",local_setnum);夫进程中定义的local_setnum是局部变量，
		//子进程无法访问
		printf("son process read start:%d, end:%d\n",start,end);
		//close(fd[0]);
	}
	return 0;
}
