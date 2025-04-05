#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>

/*
 *UDP广播发送端实现
 */

int main(int argc,char* argv[])
{
	if(argc<3){
		fprintf(stderr,"usage:%s ip(port)\n",argv[0]);
		exit(1);
	}
	/*创建套接字*/
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(1);
	}
	/*设置套接字选项为广播发送*/
	int opt=1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&opt,sizeof(opt))<0){
		perror("setsockopt error");
		exit(1);
	}
	/*发送广播数据*/
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(clientaddr));
	clientaddr.sin_family=AF_INET;
	clientaddr.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&clientaddr.sin_addr.s_addr);
	char buffer[20]="yansitiaoyu";
	if(sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&clientaddr,sizeof(clientaddr))<0){
		perror("sendto error");
		exit(1);
	}else{
		printf("broadcast success...\n");
	}
	close(sockfd);
	return 0;
}
