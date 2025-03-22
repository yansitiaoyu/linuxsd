#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
	pid_t pid=fork();//pid_t是进程数据类型，fork()函数返回值是两个，当调用fork()函数后，
	                 //后续程序便进入父进程和子进程两个分支运行
	if(pid<0){
		printf("fork error\n");
		exit(0);
	}
	else if(pid>0){//pid>o，是父进程执行的代码
		printf("%d is parent process,parent pid is %d,pid is:%d\n",getpid(),getppid(),pid);
	}
	else{//pid=0是子进程执行的代码
		printf("%d is son process,parent pid is %d,pid is:%d\n",getpid(),getppid(),pid);
	}
	//所有的代码父子进程都会执行，知道程序结束
	printf("%d is executing\n",getpid());
	sleep(1);
	return 0;
}
