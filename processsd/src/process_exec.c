#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

//execl函数族的运用

int main()
{
	char *cmd1="cat";//相对路径,execl()函数中只能用绝对路径,execlp()中两者都行;
	char *cmd2="bin/process_file";//绝对路径
	char *argv1="test";
	pid_t pid;
	if(fork()<0){
		perror("fork error");
		exit(1);
	}else if(pid==0){
		if(execl(cmd2,cmd2,argv1,NULL)<0){//execl()函数参数列表最后必须用NULL结尾
			perror("execul error");
			exit(1);
		}
	}else{
		wait(NULL);
	}
	return 0;
}
