#include <unistd.h>
#include <stdio.h>

int main()
{
	printf("pid:%d\n",getpid());
	printf("uid:%d\n",getuid());
	printf("euid:%d\n",geteuid());
	printf("group id:%d\n",getgid());
	printf("process group id:%d\n",getpgrp());
	printf("pgid:%d\n",getpgid(getpid()));
	printf("ppgid:%d\n",getpgid(getppid()));
	return 0;
}
