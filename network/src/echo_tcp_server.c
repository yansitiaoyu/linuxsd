#include "msg.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <memory.h>
#include <time.h>
#include <wait.h>

/*
 *子进程解决TCP高并发通信
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
	//子进程结束退出信号
	if(signo==SIGCHLD){
		printf("childen peocess deaded...\n");
		wait(0);
	}
}
void out_addr(struct sockaddr_in *clientaddr){
	char ip[16];
	memset(ip,0,sizeof(ip));
	short port=ntohs(clientaddr->sin_port);
	inet_ntop(AF_INET,&clientaddr->sin_addr.s_addr,ip,sizeof(ip));
	printf("client %s(%d) success connect\n",ip,port);
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

/*
void do_service(int fd)
{
	long t=time(0);
	char *buff=ctime(&t);
	size_t len=strlen(buff);
	if(write_msg(fd,buff,len)<0){
		perror("write error");
		exit(1);
	}
}
*/

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
	struct sockaddr_in clientaddr;
	memset(&clientaddr,0,sizeof(clientaddr));
	socklen_t len=sizeof(clientaddr);
	//pid_t pid;
	//int fd;
	while(1){
		int fd=accept(sockfd,(struct sockaddr*)&clientaddr,&len);
		if(fd<0){
			perror("accept error");
			exit(1);
		}
			pid_t pid=fork();
			if(pid<0){
				//perror("fork error");
				//exit(1);
				continue;
			}else if(pid>0){
				close(fd);//关闭父进程的fd
				//continue;
			}else{
				out_addr(&clientaddr);
				do_service(fd);
				close(fd);//关闭子进程复制得来的fd
				break;
			}
	}
	return 0;
}
