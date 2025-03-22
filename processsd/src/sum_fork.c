#include "process.h"
#include <stdio.h>
#include <string.h>

//用来验证父进程中创建的子进程是否会执行外部函数
void sum(int a,int b){
	printf("........a+b=%d........\n",a+b);
}
