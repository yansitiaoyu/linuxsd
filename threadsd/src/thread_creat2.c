#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//改进进程创建时，pthread_create传入的第四个参数为结构体

typedef struct{
	const char *name;
	//int  tid;
	int length;
}Race;

void* start_attr(void* arg){
	Race *r=(Race*) arg;
	for(int i=0;i<r->length;i++){
		printf("%s(%lx) run %d\n",r->name,pthread_self(),i);
		sleep(1);
	}
	return (void*)0;
}

int main()
{
	//int rabbit,turtle;//线程的类型定义是pthread_t
	pthread_t rabbit,turtle;

	//struct Race R_a={"rabbit",20};
    //struct Race R_t={"turtle",20};//这是错误定义，没有struct;
	
    Race R_a={"rabbit",20};
	Race R_t={"turtle",20};
	if(pthread_create(&rabbit,NULL,start_attr,(void*)&R_a)!=0){
		perror("pthread_create rabbit error");
	}
	if(pthread_create(&turtle,NULL,start_attr,(void*)&R_t)!=0){
		perror("pthread_create turtle error");
	}

	pthread_join(rabbit,NULL);
	pthread_join(turtle,NULL);
	printf("main thread %lx terminal\n",pthread_self());
	return 0;
}
