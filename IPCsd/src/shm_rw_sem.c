#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>

/*
 *信号集实现共享内存读写同步互斥
 */

//定义共享资源
typedef struct{
	int val;
	int semid;
}Storage;

//初始化信号集
void init(Storage* s){
	assert(s!=NULL);
	//创建信号集
	s->semid=semget(IPC_PRIVATE,2,IPC_CREAT|IPC_EXCL|0777);
	if(s->semid<0){
		perror("semget error");
		exit(1);
	}
	//联合体初始化信号集,联合体需要自己定义
	union semun{
		int val;
		struct semid_ds *ds;
		unsigned short *array;
	};
	union semun un;
	unsigned short array[2]={0,0};
	un.array=array;
	if(semctl(s->semid,0,SETALL,un)<0){
		perror("semctl error");
		exit(1);
	}
}

//销毁信号集
void destroy(Storage *s){
	assert(s!=NULL);
	if(semctl(s->semid,0,IPC_RMID,NULL)<0){
		perror("semctl destroy error");
		exit(1);
	}
}


//共享内存写入数据并vp操作通知读进程
void writer(Storage *s,int value){
	assert(s!=NULL);
	s->val=value;
	printf("pid %d writr %d\n",getpid(),s->val);
	//定义pv操作,一个完整的p或v操作都是封装在一个结构体内
	//struct sembuf* sem_v[1]={{0,1,SEM_UNDO}};
	//struct sembuf* sem_p[1]={{1,-1,SEM_UNDO}};
	struct sembuf sem_v[1]={{0,1,SEM_UNDO}};
	struct sembuf sem_p[1]={{1,-1,SEM_UNDO}};
	//v操作
	if(semop(s->semid,sem_v,1)<0){
		perror("writer  semop_v error");
		exit(1);
	}
	//p操作
	if(semop(s->semid,sem_p,1)<0){
		perror("writer semop_p error");
		exit(1);
	}
}


//共享内存读数据
void reader(Storage *s){
	assert(s!=NULL);
	//封装信号灯的pv操作
	//struct sembuf *sem_p[1]={{0,-1,SEM_UNDO}};
	//struct sembuf *sem_v[1]={{1,1,SEM_UNDO}};
	struct sembuf sem_p[1]={{0,-1,SEM_UNDO}};
	struct sembuf sem_v[1]={{1,1,SEM_UNDO}};
	//读之前p操作
	if(semop(s->semid,sem_p,1)<0){
		perror("reader semop_p error");
		exit(1);
	}
	int val=s->val;
	printf("pid %d read %d\n",getpid(),val);
	//读结束v操作
	if(semop(s->semid,sem_v,1)<0){
		perror("reader semop_v error");
		exit(1);
	}
}


int main()
{
	//创建共享内存
	int shmid=shmget(IPC_PRIVATE,sizeof(Storage),IPC_CREAT|IPC_EXCL|0777);
	if(shmid<0){
		perror("shmget error");
		exit(1);
	}
	//共享内存绑定父进程虚拟地址
	Storage* addr=(Storage*)shmat(shmid,0,0);//第二个参数0是指定绑定的内存,0表示默认,
	                                         //第三个0是跟随第二个0
	if(addr==(Storage*)-1){
		perror("shmat error");
		exit(1);
	}
	//初始化共享内存addr
	init(addr);
	//fork()创建子进程
	pid_t pid;
	if((pid=fork())<0){
		perror("fork error");
		exit(1);
	}else if(pid>0){
		for(int i=0;i<20;i++){
			writer(addr,i);
		}
		wait(NULL);
		destroy(addr);
		shmctl(shmid,IPC_RMID,NULL);
	}else{
		for(int i=0;i<20;i++){
			reader(addr);
		}
	}
	return 0;
}













































