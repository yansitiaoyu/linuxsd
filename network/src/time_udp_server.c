#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <signal.h>

/*
 *udp通信
 */

int sockfd;

void sig_handler(int signo)
{
	if(signo==SIGINT){
		printf("UDP server close\n");
		close(sockfd);
		exit(1);
	}
}

void out_address(struct sockaddr_in *clientaddr)
{
	char ip[16];
	memset(ip,0,sizeof(ip));
  	inet_ntop(AF_INET,&clientaddr->sin_addr.s_addr,ip,sizeof(ip));//->运算符的优先级是高于&的
	int port=ntohs(clientaddr->sin_port);
	printf("client %s(%d)\n",ip,port);
}

void do_service(){
	struct sockaddr_in clientaddr;
	socklen_t len=sizeof(clientaddr);
	memset(&clientaddr,0,sizeof(clientaddr));
	char buffer[1024];
	memset(buffer,0,sizeof(buffer));
	if(recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clientaddr,&len)<0){
		perror("recvfrom error");
		exit(1);
	}else{
		out_address(&clientaddr);
		//do_service();
		//close(fd);
		printf("client send info:%s\n",buffer);
		//向客服端发送数据
	    long int t=time(0);
	    char *buff=ctime(&t);
	    size_t len=strlen(buff);
	    if(sendto(sockfd,buff,len,0,(struct sockaddr*)&clientaddr,sizeof(clientaddr))<0){
			perror("sendto error");	
		}
	}
	//time_t t=time(0);
	/*
	   long int t=time(0);
	char *buff=ctime(&t);
	size_t len=strlen(buff);
	if(sendto(sockfd,buff,len,0,&clientaddr,sizeof(clientaddr))<0){
		perror("sendto error");
	}
	*/
}

//int sockfd;sockfd必须定义到所有函数之前

int main(int argc,char *argv[])
{
	if(argc<2){
		printf("usage %s port\n",argv[0]);
		exit(1);
	}
	//注册SIGINT信号
	if(signal(SIGINT,sig_handler)==SIG_ERR){
		perror("signal error");
	}
	//创建套接字	
	sockfd=socket(AF_INET,SOCK_DGRAM,0);//sockfd应该定义在主函数外面		
	if(sockfd<0){
		perror("socket error");
		exit(0);
	}
	//设置套接字选项
	int opt=1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))<0){
		perror("setsockopt error");
		exit(1);
	}
	//套接字绑定地址
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(atoi(argv[1]));
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("bind error");
		exit(1);
	}
	//和客服端进行双向的数据通信
	while(1){
		do_service();
	}

	//接收客服端消息
	/*
	   struct sockaddr_in clientaddr;
	size_t len=sizeof(clientaddr);
	memset(&clientaddr,0,sizeof(clientaddr));
	char buffer[1024];
	memset(buffer,0,sizeof(buffer));
	if(recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clientaddr,&len)<0){
		perror("recvfrom error");
		exit(1);
	}else{
		out_address(&clientaddr);
		do_service();
		close(fd);
	}
	 */
}

