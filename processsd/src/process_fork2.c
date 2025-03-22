#include "process.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int global_a=5;//定义全局变量；
int main()
{
	int local_a=10;//定义局部变量；
	int static static_a=20;//定义静态变量；

    //fork()之前程序代码段，验证过后这里子进程无法执行到
	for(int i=0;i<3;i++){
		printf("fork front, pid is:%d, global_a is:%d, local_a is:%d, static_a is:%d\n",
				getpid(),global_a,local_a,static_a);
	}
    //fork()代码段
	pid_t pid=fork();

	for(int i=0;i<3;i++){
		printf("fork behind, pid is:%d, global_a is:%d, local_a is:%d, static_a is:%d\n",
				getpid(),global_a,local_a,static_a);
		sum(10,20);
	}

	return 0;
}
