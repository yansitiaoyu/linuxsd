#include "msg.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>

/*
 * 自定义协议msg客服端实现进程通信
 */

int main(int argc,char* argv[])
{
	if(argc<3){
		fprintf(stderr,"usage:%s ip(port)\n",argv[0]);
		exit(1);
	}
	/*创建套接字*/
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(1);
	}
	/*申请客服端连接*/
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&serveraddr.sin_addr.s_addr);
	//int fd;connect()函数的返回值不是套接字fd，而是0或-1；
	//if((fd=connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr)))<0){
	if(connect(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("connect error");
		exit(1);
	}
	/*使用自定义write_msg/read_msg与客服端通信*/
	/*
	char buffer[40]="this is client require...";
	if(write_msg(fd,buffer,sizeof(buffer))<0){
		perror("protocol error");
		exit(1);
	}
	char buff[512];
	memset(buff,0,sizeof(buff));
	if(read_msg(sockfd,buff,sizeof(buff))<0){
		perror("protocol error");
		exit(1);
	}else{
		printf("%s\n",buff);
	}
	 */
	char buff[512];
	//memset(buff,0,sizeof(buff));
	size_t size;
	char* optmpt=">";
	while(1){
		memset(buff,0,sizeof(buff));
		//char* optmpt=">";
		write(STDOUT_FILENO,optmpt,1);
		size=read(STDIN_FILENO,buff,sizeof(buff));
		if(size<0) continue;
		//{
		//	perror("read error");
		//	exit(1);
		//}
		buff[size-1]='\0';

		if(write_msg(sockfd,buff,sizeof(buff))<0){
			perror("write message error");
			continue;
		}else{
			if(read_msg(sockfd,buff,sizeof(buff))<0){
				perror("raad message error");
				continue;
			}else{
				printf("%s\n",buff);
			}
		}
	}
	close(sockfd);
	return 0;
}
