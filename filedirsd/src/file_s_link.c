#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

/*
   symlink(const char *oldpathname,const char *newpathname)创建软连接，linkread(const char *newpathname)查看软链接的i节点指向的数据块内的文件路径
 */

int main(int argc,char *argv[])
{
	if(argc<3){
		fprintf(stderr,"usage:%s\n",argv[0]);
		exit(1);
	}
	if(symlink(argv[1],argv[2])){
		perror("symlink error\n");
		exit(1);
	}
	char buff[1024];
	ssize_t size;
	memset(buff,0,sizeof(buff));
	//文件读写三步骤，open()通过文件路径得到文件描述符，read(),write()通过文件描述符读写
	int fd=open(argv[2],O_RDONLY);
	if(fd<0){
		perror("open error\n");
		exit(1);
	}
	if((size=read(fd,buff,sizeof(buff)))<0){
		perror("read error\n");
		exit(1);
	}
	else{
		if(write(STDOUT_FILENO,buff,size)!=size){
			perror("writr error\n");
		    exit(1);
		}
	}
	printf("\n");

	//通过readlink()读取软连接的文件文件内容

	memset(buff,0,sizeof(buff));//初始化缓冲区

	if(readlink(argv[2],buff,sizeof(buff))<0){
		perror("readlink error\n");
		exit(1);
	}
	else{
		if(write(STDOUT_FILENO,buff,size)!=size){
			perror("write error\n");
			exit(1);
		}
	}
	printf("\n");
	/*else{
		linkread(STDOUT_FILENO,buff,sizeof(buff));
	}*/

	return 0;
}
