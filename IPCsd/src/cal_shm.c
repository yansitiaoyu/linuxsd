#include "pipe_shm_syn.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 *父子进程之间共享内存实现通信
 */

int main()
{
	init();
	//声明共享内存id
	int shmid;
	//声明共享内存key
	key_t key=atoi("10");
	//创建共享内存
	if((shmid=shmget(key,1024,IPC_CREAT|0777))<0){
		perror("shmget error");
	}
	//fork()创建子进程
	pid_t pid;
	if((pid=fork())<0){
		perror("fork error");
		exit(1);
	}else if(pid>0){
		//父进程创建共享内存（共享内存只能在进程外创建）
		/*if((shmid=shmget(key,1024,IPC_CREAT|0777))<0){
			perror("fprocess shmget error");
		}*/
		int* addr;
		//共享内存绑定父进程虚拟内存
		if((addr=(int*)shmat(shmid,0,0))==(int*)-1){
			perror("fprocess shmat error");
		}
		//父进程数据写入管道，子进程等待
		//notify_pipe();先解除映射再通知子进程访问共享内存
		addr[0]=10,addr[1]=100;
		shmdt((void*)addr);
		notify_pipe();
		destroy_pipe();
		wait(NULL);
		shmctl(shmid,SHM_DEST,NULL);
	}else{
		//子进程获得虚拟内存
		/*if(shmget(key,1024,IPC_CREAT|0777)<0){
			perror("sprocess shmget error");
		}*/

		//共享内存绑定子进程虚拟内存
		int* addr;
		if((addr=(int*)shmat(shmid,0,0))==(int*)-1){
			perror("sshmat perror");
		}
		//查看管道数据是否同步
		wait_pipe();
		printf("addr[0]:%d addr[1]:%d\n",addr[0],addr[1]);
		//销毁管道
		destroy_pipe();
		//接触共享内存和子进程的绑定
		shmdt((void*)addr);
	}
	return 0;
}
