#include "io.h"
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{ 
	if(argc != 3){
		fprintf(stderr,"suage: %s srcfile destfile\n",argv[0]);
		exit(1);
	}
	int fdin,fdout;

	//打开一个读文件
	fdin=open(argv[1],O_RDONLY);
	if(fdin<0){
		fprintf(stderr,"open error:%s",strerror(errno));
		exit(1);
	}
	else{
		printf("open file:%d\n",fdin);
	}

	//打开一个待写入的文件
	fdout=open(argv[2],O_WRONLY | O_CREAT | O_TRUNC,0777);
	if(fdout<0){
		fprintf(stderr,"open error:%s",strerror(errno));
		exit(1);
	}
	else{
		printf("open file:%d\n",fdout);
	}

	//复制文件后并关闭文件
	copy(fdin,fdout);
	close(fdin);
	close(fdout);

	return 0;
}
