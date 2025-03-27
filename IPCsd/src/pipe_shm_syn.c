#include "pipe_shm_syn.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

static int fd[2];

//初始化管道,管道默认是阻塞的
void init(){
	//int fd[2];这里不能定义为局部变量
	//if(pipe(fd[2])<0),pipe()中的参数是数组名，不是整个数组
	if(pipe(fd)<0){
		printf("pipe error");
	}
}
//等待管道消息
void wait_pipe(){
	char c;
	if(read(fd[0],&c,1)<0){
		perror("read error");
	}
}
//向管道发送消息
void notify_pipe(){
	char c='c';
	if(write(fd[1],&c,1)!=1){
		perror("write error");
	}
}
//关闭管道
void destroy_pipe(){
	close(fd[0]);
	close(fd[1]);
}


