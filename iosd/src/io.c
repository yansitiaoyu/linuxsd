/*
#include "io.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

# define BUFFER_LEN 4096
void copy(int fdin,int fdout)
{
	char buffer[BUFFER_LEN];
	ssize_t size;
	//while(size=read(fdin,buffer,BUFFER_LEN)>0);
	//上面的代码少了一个括号，>运算符的优先级大于=，所以复制出错了;
	while((size=read(fdin,buffer,BUFFER_LEN))>0){
		if(write(fdout,buffer,size)!=size){
			fprintf(stderr,"write error:%s",strerror(errno));
			exit(1);
		}
	}
	if(size<0){
		fprintf(stderr,"read error:%s",strerror(errno));
		exit(1);
	}
}
*/



#include "io.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define BUFFER_LEN 4096

void copy(int fdin, int fdout) {
    char buffer[BUFFER_LEN];
    ssize_t size;

    while ((size = read(fdin, buffer, BUFFER_LEN)) > 0) {
        if (write(fdout, buffer, size) != size) {
            fprintf(stderr, "write error: %s\n", strerror(errno));
            exit(1);
        }
    }

    if (size < 0) {
        fprintf(stderr, "read error: %s\n", strerror(errno));
        exit(1);
    }
}


//设置文件状态标志
void set_fl(int fd,int flag)
{
	//获得原来的文件状态标志
	int val=fcntl(fd,F_GETFL);
	//增加新的文件状态标志
	val |=flag;
	//设置新的文件状态标志
	if((fcntl(fd,F_SETFL,val))<0){
		perror("fnctl 错误\n");
	}
}	

void ctl_fl(int fd,int flag)
{
	int val=fcntl(fd,F_GETFL);
	//清除指定文件状态标志
	val &= ~flag;
	//重新设置清楚后的文件状态标志
	if(fcntl(fd,F_SETFL,val)<0){
		perror("fcntl 错误\n");
	}
}


