#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>


/*
 实现判断文件类型命令，文件类型总共有7种
 */
int main(int argc,char *argv[])
{
	if(argc<2){
		fprintf(stderr,"usage:%s file\n",argv[0]);
	    exit(1);
	}
	struct stat buff;
	for(int i=1;i<argc;i++){
		memset(&buff,0,sizeof(buff));
		if(stat(argv[i],&buff)!=0){
			perror("stat error\n");
			continue;
		}
		printf("%-20s",argv[i]);
		if(S_ISREG(buff.st_mode)){
			printf("nomal file");
		}
		else if(S_ISDIR(buff.st_mode)){
			printf("directory file");
		}
		else if(S_ISFIFO(buff.st_mode)){
			printf("pipe file");
		}
		else if(S_ISBLK(buff.st_mode)){
			printf("block file");
		}
		else if(S_ISLNK(buff.st_mode)){
			printf("link file");
		}
		else if(S_ISCHR(buff.st_mode)){
			printf("charactor device");
		}
		else if(S_ISSOCK(buff.st_mode)){
			printf("sock device");
		}
		else{
			printf("unknow");
		}
		printf("\n");
	}
	return 0;
}
