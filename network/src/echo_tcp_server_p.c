#include "msg.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <memory.h>
#include <stdint.h>

/*
 *多线程解决TCP高并发通信
 */

int sockfd;

void sig_handler(int signo)
{
	//退出信号
	if(signo==SIGINT){
		printf("server end...\n");
		close(sockfd);
		exit(0);
	}
}

/*
   void out_addr(struct sockaddr_in *clientaddr){
	char ip[16];
	memset(ip,0,sizeof(ip));
	short port=ntohs(clientaddr->sin_port);
	inet_ntop(AF_INET,&clientaddr->sin_addr.s_addr,ip,sizeof(ip));
	printf("client %s(%d) success connect\n",ip,port);
}
*/
void out_fd(int fd)
{
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(clientaddr));
	socklen_t len=sizeof(clientaddr);
	if(getpeername(fd,(struct sockaddr*)&clientaddr,&len)<0){
		perror("getpeername error");
		return;
	}
	char ip[16];
	memset(ip,0,sizeof(ip));
	short port=ntohs(clientaddr.sin_port);
	inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ip,sizeof(ip));
	printf("%s(%d) thread:%ld closed!\n",ip,port,pthread_self());
}

void do_service(int fd)
{
	char buff[512];
	//size_t size;
	while(1){
		printf("start read and write...\n");
		memset(buff,0,sizeof(buff));
		size_t size=read_msg(fd,buff,sizeof(buff));
		if(size<0){
			perror("read protocol error");
			//continue;
			break;
		}else if(size==0){//双通管道写段关闭，read返回0
			break;
		}else{
			printf("%s\n",buff);
			if(write_msg(fd,buff,sizeof(buff))<0){
				//perror("write protocol error");
				if(errno==EPIPE){
					break;
				}
				perror("write protocol error");
			}
			   //perror("write protocol error");
			}
		}
}

//定义线程启动函数
void* start_attr(void* arg)
{
	intptr_t fd=(intptr_t) arg;
	do_service(fd);
	out_fd(fd);
	close(fd);
	return (void*) 0;
}

int main(int argc,char* argv[])
{
	if(argc<2){
		fprintf(stderr,"usage:%s port\n",argv[0]);
		exit(0);
	}
	/*登记信号*/
	if(signal(SIGINT,sig_handler)==SIG_ERR){
		perror("signal sigint error");
		exit(1);
	}
	if(signal(SIGCHLD,sig_handler)==SIG_ERR){
		perror("signal sigchld error");
		exit(1);
	}
	/*创建套接字*/
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		perror("socket error");
		exit(1);
	}
	/*套接字绑定地址*/
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(atoi(argv[1]));
	serveraddr.sin_addr.s_addr=INADDR_ANY;
	if(bind(sockfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0){
		perror("bind error");
		exit(1);
	}
	/*监听端口请求*/
	if(listen(sockfd,10)<0){
	    perror("lisent error");
	    exit(1);
	}
	/*接受连接请求*/
	//struct sockaddr_in clientaddr;
	//memset(&clientaddr,0,sizeof(clientaddr));
	//socklen_t len=sizeof(clientaddr);

	/*设置主控线程分离属性*/
	pthread_attr_t attr;
	//初始化线程属性
	pthread_attr_init(&attr);
	//设置分离属性
	if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)<0){
		perror("pthread_attr_setdetachstat error");
		exit(1);
	}
	while(1){
		//int fd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
		int fd=accept(sockfd,NULL,NULL);//通过套接字fd获得客服端地址信息
		if(fd<0){
			perror("accept error");
			continue;
		}
		//创建线程
	    pthread_t tid;
		//if(pthread_create(&tid,&attr,start_attr,(void*)fd)!=0){
		if(pthread_create(&tid,&attr,start_attr,(void*)(intptr_t)fd)!=0){//对于简单场景类型转换需要使用intptr_t来安全转换
			perror("pthread_create error");
			exit(1);
		}
		//销毁线程分离属性
		pthread_attr_destroy(&attr);
	}
	//pthread_attr_destroy(&attr);
	return 0;
}
