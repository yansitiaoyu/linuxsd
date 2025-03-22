#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
//线程终止，pthread_join()回收线程资源

typedef struct{
	int a;
	int b;
}Test;

void* start_attr(void *arg){
	Test *T=(Test*) arg;
	return (void*)(intptr_t) ((T->a)+(T->b));//intptr_t保证指针和整数之间安全强制类型转换
}

int main()
{
	pthread_t p1;
	Test T={10,20};

	if(pthread_create(&p1,NULL,start_attr,(void*) &T)!=0){
		perror("pthread_create error");
	}

	void* ret;
	pthread_join(p1,&ret);
	printf("%d\n",(int)(intptr_t)ret);

	return 0;
}
