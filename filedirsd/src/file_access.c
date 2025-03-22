#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

/*
   判断文件是否具有R_OK,W_OK,X_OK权限，F_OK判断文件是否存在
 */

int main(int argc,char *argv[])
{
	if(argc<0){
		fprintf(stderr,"usage:%s\n",argv[0]);
		exit(1);
	}
	for(int i=1;i<argc;i++){
		if(access(argv[i],F_OK)){
			printf("%s not exist\n",argv[i]);
			continue;
		}
		if(access(argv[i],R_OK)){
			printf("%d not read %s\n",getpid(),argv[i]);
		}
		else{
			printf("%d can read %s\n",getpid(),argv[i]);
		}
        if(access(argv[i],W_OK)){
            printf("%d not write %s\n",getpid(),argv[i]);
        }
        else{
            printf("%d can write %s\n",getpid(),argv[i]);
        }
		if(access(argv[i],X_OK)){
			printf("%d not execute %s\n",getpid(),argv[i]);
		}
		else{
			printf("%d can execute %s\n",getpid(),argv[i]);
		}
		printf("\n");
	}
	return 0;
}
