#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

char *buffer="0123456789";

int main(int argc,char *argv[])
{
	if(argc<2){
		fprintf(stderr,"usage:%s[file]\n",argv[0]);
		exit(1);
	}
//open得到文件描述符
int fd=open(argv[1],O_WRONLY | O_CREAT | O_TRUNC,0777);
if(fd<0){
	perror("open fail\n");
	exit(1);
}
ssize_t size=strlen(buffer)*sizeof(char);
//写文件
if(write(fd,buffer,size)!=size){
	perror("write fail\n");
	exit(1);
}

//通过lseek函数定位到文件结尾后十个字节处得到空洞文件再写入
if(lseek(fd,10,SEEK_END)<0){
	perror("lseek error\n");
	exit(1);
}
//再次写入文件
if(write(fd,buffer,size)!=size){
	perror("write error\n");
	exit(1);
}
    close(fd);
	return 0;
}
