#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <memory.h>

/*
 * UDP广播通信接收端实现
 */

int sockfd;

void sig_handler(int signo)
{
	if(signo==SIGINT){
		printf("recevier closed\n");
		close(sockfd);
		exit(0);
	}
}

int main(int argc, char *argv[])
{
	if(argc<2){
		fprintf(stderr,"usage:%s port\n",argv[0]);
		exit(1);
	}
	/*信号注册*/
	if(signal(SIGINT,sig_handler)==SIG_ERR){
		perror("signal error");
	}
	/*创建套接字*/
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(1);
	}
	/*套接字绑定端口地址*/
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(atoi(argv[1]));
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("bind error");
		exit(0);
	}
	/*接受广播发送的数据*/
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(clientaddr));
	socklen_t len=sizeof(clientaddr);
	char buffer[1024];
	memset(buffer,0,sizeof(buffer));
	while(1){
		if(recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clientaddr,&len)<0){
			perror("recvfrom error");
			exit(1);
		}else{
			char ip[16];
			memset(ip,0,sizeof(ip));
			inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip));
			int port=ntohs(clientaddr.sin_port);
			//printf("%s(%d):%s",ip,port,buffer);输出函数中若是没有\n会导致缓冲区未刷新，而不能自动输出，只能回车后才输出到屏幕
			printf("%s(%d):%s\n",ip,port,buffer);
	}
	return 0;
}
