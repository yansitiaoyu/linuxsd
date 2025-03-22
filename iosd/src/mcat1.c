#include "io.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/*通过函数dup2(fd1,fd2)函数实现mcat1的输入输出重定向
 +输入重定向，-输出重定向*/

int main(int argc, char *argv[])
{
	for(int i=1;i<argc;i++){
		if(!(strcmp(argv[i],"+"))){
			int fd_in=open(argv[++i],O_RDONLY);
			if(fd_in<0){
				perror("输入重定向打开文件错误\n");
				exit(1);
			}
			/*
			   dup2(fd_in,STDIN_FILENO);每一个有返回值的函数都应该判断返回值是                否正确
			 */
			if(dup2(fd_in,STDIN_FILENO)!=STDIN_FILENO){
				perror("dup2错误\n");
				exit(1);
			}
			copy(STDIN_FILENO,STDOUT_FILENO);
			close(fd_in);
		}
		else if(!(strcmp(argv[i],"-"))){
			int fd_out=open(argv[++i],O_WRONLY | O_CREAT | O_TRUNC,0777);
			if(fd_out<0){
				perror("输出重定向打开文件错误\n");
				exit(1);
			}
			/*判断dup2返回值知否正确*/
			if((dup2(fd_out,STDOUT_FILENO))!=STDOUT_FILENO){
				perror("dup2错误\n");
				exit(1);
			}
			copy(STDIN_FILENO,STDOUT_FILENO);
			close(fd_out);
		}
		else{
			copy(STDIN_FILENO,STDOUT_FILENO);
		}
	}
	return 0;
}
