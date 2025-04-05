#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <memory.h>

/*
 *udp通信客服端实现
 */

int main(int argc,char *argv[])
{
	if(argc<3){
		printf("usage %s ip port\n",argv[0]);
		exit(1);
	}
	/*创建套接字*/
	int sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(1);
	}
	/*调用recvfrom与sendto与客服端通信*/
	struct sockaddr_in serveraddr;
	//socket_t len=sizeof(serveraddr);
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&serveraddr.sin_addr.s_addr);

	char buffer[20]="yansitiaoyu";
	if(sendto(sockfd,buffer,strlen(buffer),0,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("sendto error");
		exit(1);
	}else{
		char buff[1024];
		memset(&buff,0,sizeof(buff));
		socklen_t len=sizeof(serveraddr);
		if(recvfrom(sockfd,buff,sizeof(buff),0,(struct sockaddr*)&serveraddr,&len)<0){
			perror("recvfrom error");
			exit(1);
		}else{
			printf("%s\n",buff);
		}
	}
	close(sockfd);
	return 0;
}
