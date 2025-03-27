#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

/*
 *创建一个消息队列，并向消息队列中发送消息
 */

//定义消息结构体
typedef struct
{
	long type;//消息类型
	int start;//消息数据本身，包括start和end
	int end;
}MSG;

int main(int argc,char *argv[])
{
	if(argc<2){
		printf("usage:%s key\n",argv[0]);
		exit(1);
	}
	//声明并获得消息队列key
	key_t key;
	key=atoi(argv[1]);
	printf("msg key:%d\n",key);
	//声明消息队列id
	int msq_id;
	//创建消息队列
	if((msq_id=msgget(key,IPC_CREAT|IPC_EXCL|0777))<0){
		perror("msgget error");
		exit(1);
	}
	//定义并发送消息
	MSG m1={1,10,20};
	MSG m2={2,20,30};
	MSG m3={3,30,40};
	MSG m4={4,40,80};
	MSG m5={4,40,100};
	if(msgsnd(msq_id,&m1,sizeof(m1)-sizeof(long),IPC_NOWAIT)<0){
		perror("send error");
	}
	if(msgsnd(msq_id,&m2,sizeof(m2)-sizeof(long),IPC_NOWAIT)<0){
		perror("send error");
	}
	if(msgsnd(msq_id,&m3,sizeof(m3)-sizeof(long),IPC_NOWAIT)<0){
		perror("send error");
	}
	if(msgsnd(msq_id,&m4,sizeof(m4)-sizeof(long),IPC_NOWAIT)<0){
		perror("send error");
	}
	if(msgsnd(msq_id,&m5,sizeof(m5)-sizeof(long),IPC_NOWAIT)<0){
		perror("send error");
	}
	//通过消息属性结构体获得消息队列中的消息总数
	struct msqid_ds ds;
	if(msgctl(msq_id,IPC_STAT,&ds)<0){
		perror("magctl error");
	}
	printf("msg total:%ld\n",ds.msg_qnum);

	return 0;
}
