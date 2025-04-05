#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <time.h>

/*
 *实现tcp返回时间服务器
 */

int sockfd;

void sig_hander(int signo){
	if(signo==SIGINT){
		printf("server close\n");
		//关闭服务器端socket
		close(sockfd);
		exit(0);
	}
}

//输出连接的客服端的地址信息
void out_clientaddr(struct sockaddr_in *clientaddr){
	short port=ntohs(clientaddr->sin_port);
	char ip[16];
	memset(ip,0,sizeof(ip));
	inet_ntop(AF_INET,&clientaddr->sin_addr.s_addr,ip,sizeof(ip));
	printf("client %s(%d) connected\n",ip,port);
}

//客服端请求处理
void do_service(int fd){
	time_t t=time(0);
	char *s=ctime(&t);
	size_t size=strlen(s);
	write(fd,s,size);
}

int main(int argc, char *argv[])
{
	if(argc<2){
		printf("usage %s port\n",argv[0]);
		exit(1);
	}
	//登记SIGINT信号，用于ctrl+c退出程序
	if(signal(SIGINT,sig_hander)==SIG_ERR){
		perror("signal error");
		exit(1);
	}
	//创建套接字
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(1);
	}
	//创建因特网地址
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	//往英特网地址中填入地址族,ip,端口信息
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(atoi(argv[1]));
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	//绑定sockfd和地址
	if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("bind error");
		exit(1);
	}
	//服务器监听客服端请求
	if(listen(sockfd,10)<0){
		perror("listen error");
		exit(1);
	}
	/*
	 *accept接受客服端请求，返回客服端sockfd
	 *struct sockaddr_in 地址接受客服端地址
	 *没有客服端连接调用此函数会阻塞
	 */
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len=sizeof(clientaddr);
	while(1){
		int fd=accept(sockfd,(struct sockaddr*)&clientaddr,&clientaddr_len);
		if(fd<0){
			perror("accept error");
			continue;
		}
	/*
	 *调用I/O函数(read/write)和连接的客服端进行双向通信
	 *while循环时每次获得的客服端sockfd都是不同的
	 */
		out_clientaddr(&clientaddr);
		do_service(fd);
		//关闭客服端连接
		close(fd);
	}
	return 0;
}










