#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
//signal()函数登记信号和处理函数，当程序捕获到信号的一瞬间就会去执行登记的信号处理函数；

void signal_handler(int signo){
	printf("process id is %d,signal value is %d\n",getpid(),signo);
}

int main()
{
	if(signal(SIGINT,signal_handler)==SIG_ERR){
		printf("SIGINT error\n ");
	}

	if(signal(SIGTSTP,signal_handler)==SIG_ERR){
		printf("SIGTSTP error\n");
	}

	if(signal(SIGUSR1,signal_handler)==SIG_ERR){
		printf("SIGUSR1 error");
	} 

	if(signal(SIGUSR2,signal_handler)==SIG_ERR){
		printf("SIGUSR2 error");
	}
	/*if(signal(SIGKILL,SIG_IGN)==SIG_ERR){
		perror("SIGKILL error");
	}

	if(signal(SIGSTOP,SIG_IGN)==SIG_ERR){
		perror("SIGSTOP error");
	}*/

	for(int i=0;i<20;i++){
		printf("%d pid out %d\n",getpid(),i);
		if(i==10)
			kill(getpid(),SIGKILL);
		sleep(1);
	}
	raise(SIGUSR1);
	kill(getpid(),SIGUSR2);
	return 0;
}
