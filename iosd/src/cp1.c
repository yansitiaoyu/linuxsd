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

	//lseek函数用于查看文件头到SEEK_END标记的位置偏移了offset个字节后的位置
	//之间的长度
	//printf("file length:%ld\n",lseek(fdin,0,SEEK_END));
    //使用了lseek()函数后，fdin文件指针将指向文件末尾，导致read()读不到数据
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
	printf("文件复制成功\n");
	close(fdin);
	close(fdout);

	return 0;
}


/*int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        exit(1);
    }

    int fdin = open(argv[1], O_RDONLY);
    if (fdin < 0) {
        fprintf(stderr, "Failed to open %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    int fdout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fdout < 0) {
        fprintf(stderr, "Failed to open %s: %s\n", argv[2], strerror(errno));
        close(fdin);
        exit(1);
    }

    copy(fdin, fdout);

    close(fdin);
    close(fdout);

    printf("File copied successfully!\n");
    return 0;
}*/
