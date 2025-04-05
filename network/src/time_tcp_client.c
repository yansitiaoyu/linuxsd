#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <memory.h>

/*
 *tcp客服端实现对服务端的请求
 */

int main(int argc,char *argv[])
{
	if(argc<3){
		printf("usage %s ip port\n",argv[0]);
		exit(1);
	}
	//创建套接字
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("sockfd error");
		exit(1);
	}
	//获得服务器的ip和端口port
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&serveraddr.sin_addr.s_addr);//argv[1]中已经存的是指针了
	//连接服务端
	if(connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("connect error");
		exit(1);
	}
	//通过I/O(read,write)进行读写通信
	char buff[1024];
	memset(buff,0,sizeof(buff));
	size_t size;
	/*if((size=read(sockfd,buff,strlen(buff)))<0){*/   //这里使用strlen来计算读取长度是错误的，因为memset刚初始化为0，strlen()也是0
	if((size=read(sockfd,buff,sizeof(buff)))<0){
		perror("read error");
		exit(1);
	}
	if(write(STDOUT_FILENO,buff,size)!=size){
		perror("write error");
		exit(1);
	}
	close(sockfd);
	return 0;
}
