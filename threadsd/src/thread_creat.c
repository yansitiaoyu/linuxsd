#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

void* start_attr(void* arg){//线程执行程序段
	/*for(int i=0;i<20;i++){
		printf("%lu pthread run %d",pthread_self(),i);
	}*/

	intptr_t max=(intptr_t) arg;//指针类型转换为int类型为了转换安全用stdint库里定义的整数类型intptr_t
	for(int i=0;i<max;i++){
		printf("%lx pthread run %d\n",pthread_self(),i);
		//sleep(1);
	}
	return (void*) 0;
}

int main()
{ 
	//pthread_t *rabbit,*turtle;//定义线程rabbit,turtle,这种定义方法是错误的，这是未初始化的指针
	pthread_t rabbit,turtle;
	
	if(pthread_create(&rabbit,NULL,start_attr,(void*)20)!=0){
		perror("pthread_create rabbit error");
	}

	if(pthread_create(&turtle,NULL,start_attr,(void*)20)!=0){
		perror("pthread_create turtle error");
	}

	//sleep(20);
	pthread_join(rabbit,NULL);
	pthread_join(turtle,NULL);
	printf("main pthread:%lx\n",pthread_self());
	printf("main pthread termnal\n");//当主线程结束时，其他线程也会被强制结束

	return 0;
}
