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
#include <fcntl.h>
#include "vector_fd.h"

/*
 *I/O多路复用解决TCP高并发通信
 */

VectorFD *vfd;
int sockfd;

void sig_handler(int signo)
{
	//退出信号
	if(signo==SIGINT){
		printf("server end...\n");
		close(sockfd);
		destroy_vector_fd(vfd);
		exit(0);
	}
}

void out_addr(struct sockaddr_in* clientaddr)
{
	char ip[16];
	memset(ip,0,sizeof(ip));
	short port=ntohs(clientaddr->sin_port);
	inet_ntop(AF_INET,&clientaddr->sin_addr.s_addr,ip,sizeof(ip));
	printf("client%s(%d) connect...\n",ip,port);
}

/*
 *fd对应某个客服端的连接，采用非阻塞的方式通信
 */
void do_service(int fd)
{
	char buff[512];
	//因为采用非阻塞的方式。所以不用判断size小于0的情况，会直接返回连接下一个客服端
	memset(buff,0,sizeof(buff));
	size_t size=read(fd,buff,sizeof(buff));
	if(size==0){//双通管道写段关闭，read返回0
		char info[]="client closed";
		write(STDOUT_FILENO,info,sizeof(info));
		delete_fd(vfd,fd);
		close(fd);
	}else{
	    write(STDOUT_FILENO,buff,sizeof(buff));
		if(write(fd,buff,sizeof(buff))!=0){
		    if(errno==EPIPE){
			    perror("write error");
				delete_fd(vfd,fd);
				close(fd);
			}
		}
	}
}

//定义线程启动函数
void* start_attr(void* arg)
{	
	while(1){
		int i=0;
		for(;i<vfd->counter;i++){
			do_service(get_fd(vfd,i));
		}
	}
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
	/*创建放置fd的动态数组*/
	vfd=create_vector_fd();
	/*设置主控线程分离属性*/
	pthread_t tid;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)<0){
		perror("pthread_attr_setdetachstat error");
		exit(1);
	}
	if(pthread_create(&tid,&attr,start_attr,(void*)0)!=0){//对于简单场景类型转换需要使用intptr_t来安全转换
		perror("pthread_create error");
		exit(1);
	}
	//销毁线程分离属性
	pthread_attr_destroy(&attr);
	struct sockaddr_in clientaddr;
	socklen_t len=sizeof(clientaddr);
	/*
	 *1)主控线程获得新的客服端连接，将得到的fd放到动态数组中
	 *2)启动的子线程负责遍历动态数组，和其中的fd进行通信(read/write采用非阻塞的方式)
	 */
	while(1){
		int fd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
		if(fd<0){
			perror("accept error");
			continue;
		}
		out_addr(&clientaddr);
		/*将读写方式改为非阻塞*/
		int val=fcntl(fd,F_GETFL,&val);
		val |= O_NONBLOCK;
		fcntl(fd,F_SETFL,val);
		/*将返回的新描述符放入动态数组里*/
		add_fd(vfd,fd);
	}
	return 0;
}
