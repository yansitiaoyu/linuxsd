#include "semnums_pv.h"
#include <sys/sem.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>

//声明联合体用于控制信号集初始化
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short* array;
};

//创建并初始化信号集
int I(int semnums,int value){
	int semid;
	//key默认设置为IPC_PRIVATE
	semid=semget(IPC_PRIVATE,semnums,IPC_CREAT|IPC_EXCL|0777);
	if(semid<0){
		return -1;
	}
	//通过semctl初始化信号集
	union semun un;
	/*for(int i=0;i<semnums;i++){
		un.array[i]=value;
	}*/
	//malloc创建一块堆内存，不直接使用数组是为了可控数组大小，防止内存溢出
	unsigned short* array=(unsigned short*)malloc(sizeof(unsigned short)*semnums);
	for(int i=0;i<semnums;i++){
		array[i]=value;
	}
	un.array=array;

	/*
	 *  通过共用体semun初始化信号集中的所有信号灯的初始值
	 *  0表示要初始化信号集中的所有信号灯
	 */

	if(semctl(semid,0,SETALL,un)<0){
		return -1;
	}
	//释放堆内存
	free(array);
	return semid;
}

//信号集P操作,P/V操作都是通过函数semop来控制，一个完整操作都封装在结构体sembuf里
void P(int semid,int semnum,int value){
	assert(value>=0);
	struct sembuf buf[]={{semnum,value,SEM_UNDO}};
	semop(semid,buf,sizeof(buf)/sizeof(buf[0]));
}

//信号集V操作
void V(int semid,int semnum,int value){
	assert(value>=0);
	struct sembuf buf[]={{semnum,value,SEM_UNDO}};
	semop(semid,buf,sizeof(buf)/sizeof(buf[0]));
}

//信号集销毁
void D(int semid){
	semctl(semid,0,IPC_RMID,NULL);
}


