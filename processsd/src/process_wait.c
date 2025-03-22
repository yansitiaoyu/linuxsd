#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
	int status;
	pid_t pid;
	if((pid=fork())<0){
		perror("fork error:");
		exit(1);
	}
	else if(pid==0){
		exit(3);
	}
	else{
		wait(&status);
		if(WIFEXITED(status)){
			printf("normal exit:%d\n",WEXITSTATUS(status));
		}
		else if(WIFSIGNALED(status)){
			printf("abnormal exit:%d\n",WTERMSIG(status));
		}
		else if(WIFSTOPPED(status)){
			printf("abnormal exit:%d\n",WSTOPSIG(status));
		}
		else{
			printf("nuknow signal\n");
		}
	}
	return 0;
}
