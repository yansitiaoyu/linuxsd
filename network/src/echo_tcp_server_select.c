#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
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
#include <sys/time.h>

/*
 *I/O多路复用select()函数解决TCP高并发通信
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
	//printf("do_service...\n");
	char buff[512];
	//因为采用非阻塞的方式。所以不用判断size小于0的情况，会直接返回连接下一个客服端
	memset(buff,0,sizeof(buff));
	size_t size=read(fd,buff,sizeof(buff));
	if(size==0){//双通管道写段关闭，read返回0
		printf("client closed\n");
		delete_fd(vfd,fd);
		close(fd);
	}else{
	    //write(STDOUT_FILENO,buff,sizeof(buff));
		printf("%s\n",buff);
		if(write(fd,buff,sizeof(buff))!=0){
		    if(errno==EPIPE){
			    perror("write error");
				delete_fd(vfd,fd);
				close(fd);
			}
		}
	}
}

//将fd放入符号集并返回最大fd
int add_set(fd_set *set)
{
	//printf("add_set...\n");
	FD_ZERO(set);
	int i=0;
	int max_fd=vfd->fd[0];
	for(;i<vfd->counter;i++){
		int fd=get_fd(vfd,i);
		if(fd>max_fd){
			max_fd=fd;
		}
		FD_SET(fd,set);
	}
	return max_fd;
}

//定义线程启动函数
void* start_attr(void* arg)
{	
	//printf("start_attr...\n");
	//定义超时结构体
	struct timeval t;
	t.tv_sec=2;
	t.tv_usec=0;
	//定义符号集
	fd_set set;
	//将动态数组中的fd加入符号集set，并返回描述符集中最大的fd
	int maxfd=add_set(&set);
	//调用select()委托内核检查符号集中准备好的描述符
	//int n=select(maxfd+1,&set,NULL,NULL,&t);
	int n=0;
	while((n=select(maxfd+1,&set,NULL,NULL,&t))>=0){//这里的参数是maxfd+1
		if(n>0){//n>0表示文件描述符准备好了
			int i=0;
			for(;i<vfd->counter;i++){
				//int fd=get_fd(vfd,fd);这是一个因为太依赖编译器而难以发现的错误
				int fd=get_fd(vfd,i);
				if(FD_ISSET(fd,&set)){
					do_service(fd);
				}
			}
		}
		//n<0表示超时了，超时后需要重新设置时间并清空描述符集
		t.tv_sec=2;
		t.tv_usec=0;
		//重新遍历动态数组并将最新的描述符放入到描述符集中
		maxfd=add_set(&set);
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
	 * 1)主控线程获得新的客服端连接，将得到的fd放到动态数组中
	 * 2）a)启动的子线调用select()函数委托内核去检查传入到select()中的描述符是否准备好
	 *    b)利用FD_ISSET()查找准备好的描述符，并和客服端通信
	 */
	while(1){
		int fd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
		if(fd<0){
			perror("accept error");
			continue;
		}
		out_addr(&clientaddr);
		/*将返回的新描述符放入动态数组里*/
		add_fd(vfd,fd);
	}
	return 0;
}
