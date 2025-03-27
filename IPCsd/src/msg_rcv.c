#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

/*
 *从消息队列中接受消息
 */

//定义消息结构体
typedef struct
{
	long type;
	int start;
	int end;
}MSG;

int main(int argc,char* argv[])
{
	if(argc<3){
		printf("usage %s key type\n",argv[0]);
		exit(1);
	}
	//声明并创建key
	key_t key;
	key=atoi(argv[1]);
	//声明消息队列id
	int msq_id;
	if((msq_id=msgget(key,IPC_CREAT|0777))<0){
		perror("msgget error");
	}
	//从消息队列接收消息
	MSG m;
	long type=atoi(argv[2]);
	if(msgrcv(msq_id,&m,sizeof(m.start)+sizeof(m.end),type,IPC_NOWAIT)<0){
		perror("msgrcv error");
	}else{
		printf("msg %d start:%d end:%d\n",key,m.start,m.end);
	}
	return 0;
}
