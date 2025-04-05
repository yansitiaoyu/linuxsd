#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>
#include "msg.h"

static unsigned char msg_check(Msg *message)
{
	int checknum=0;
	for(int i=0;i<sizeof(message->head);i++){
		checknum+=message->head[i];
	}
	for(int j=0;j<sizeof(message->buff);j++){
		checknum+=message->buff[j];
	}
	return checknum;
}

//将客服端或服务端生成的数据用自定义协议包装后发送,如何获得len呢？
int write_msg(int sockfd,char *buff,size_t len)
{
	//定义Msg变量
	Msg message;
	//memset(&message,0,sizeof(Msg));
	memset(&message,0,sizeof(message));
	//m.head="yansitiaoyu";
	strcpy(message.head,"yansitiaoyu");//字符串拷贝，遇到\0便停止拷贝
	memcpy(message.buff,buff,len);//内存拷贝，按字节拷贝，不区分数据类型，需指定拷贝长度
	message.checknum=msg_check(&message);
	size_t size=sizeof(message);
	if(write(sockfd,&message,size)!=size){
		return -1;
	}
}

//客服端或服务端将收到的自定义协议数据读到buff里
int read_msg(int sockfd,char *buff,size_t len){
	Msg message;
	memset(&message,0,sizeof(message));
	size_t size;
	if((size=read(sockfd,&message,sizeof(message)))<0){
		return -1;
	}else if(size==0){
		return 0;
	}
	if((msg_check(&message)==(unsigned int)message.checknum) && (!strcmp(message.head,"yansitiaoyu"))){
		memcpy(buff,message.buff,len);
		return sizeof(message);
	} 
	return -1;
}

