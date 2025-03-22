#include "io.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//实现cat命令，将一个或多个文件文件输出到命令行
int main(int argc,char *argv[])
{
	int fd_in=STDIN_FILENO;//stdio头文件中定义的标准输入文件描述符0和
	                       
	int fd_out=STDOUT_FILENO;//标准输出文件描述符1
	for(int i=1;i<argc;i++){
		fd_in=open(argv[i],O_RDONLY);
		if(fd_in<0){
			perror("open error");
			continue;
		}
		copy(fd_in,fd_out);
		close(fd_in);
	}
	if(argc==1) copy(fd_in,fd_out);

	return 0;
}
