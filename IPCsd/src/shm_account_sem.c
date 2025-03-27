#include "account.h"
#include "semnums_pv.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

/*
 *共享内存以及信号量集实现父子进程同步互斥访问银行账户
 */

int main()
{
	//创建共享内存
	int shmid;
	shmid=shmget(IPC_PRIVATE,sizeof(account),IPC_CREAT|IPC_EXCL|0777);
	if(shmid<0){
		perror("shmget error");
		exit(1);
	}
	//共享内存绑定进程地址
	account *addr=(account*)shmat(shmid,0,0);
	//初始化银行账户
	addr->idcard=111111;
	addr->balance=1000.0;
	addr->semid=I(1,1);
	//创建父子进程
	pid_t pid;
	pid=fork();
	if(pid<0){
		perror("fork error");
		exit(1);
	}else if(pid>0){
		/*
		 *addr->idcard=11111111;
	     *addr->balance=10000.00;对共享内存的初始化应该在fork()之前
		 *addr->semid=semid;
		 */
		
		//父进程执行取款操作
		double amt=withdraw(addr,1000.0);
		printf("pid %d withdraw %lf from idcard %d\n",getpid(),amt,addr->idcard);
		wait(NULL);
		printf("account balance:%lf\n",addr->balance);
		//销毁信号量集
		D(addr->semid);
		//解除映射
		shmdt(addr);
		shmctl(shmid,IPC_RMID,NULL);
	}else{
		//子进程取款
		double amt=withdraw(addr,10000.00);
		printf("pid %d withdraw %lf from idcard %d\n",getpid(),amt,addr->idcard);
		//解除映射
		shmdt(addr);
	}
	return 0;
}














