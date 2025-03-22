#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
/*线程启动属性默认和分离属性方式*/

void *start_attr(void* arg){
	int sum=0;
	for(int i=1;i<(int)(intptr_t)arg;i++){
		sum+=i;
	}
	return (void*)(intptr_t)sum;
}

void out_stat(pthread_attr_t *attr){
	int detach_stat;
	pthread_attr_getdetachstate(attr,&detach_stat);
	printf("detach stat: %d\n",detach_stat);
}

int main()
{
	//定义子线程
	pthread_t default_pt,detach_pt;
	//定义线程结构体
	pthread_attr_t attr;
	//初始化线程属性
	pthread_attr_init(&attr);
	//获得分离属性，以默认方式启动子线程
	if(pthread_create(&default_pt,&attr,start_attr,(void*) 20)!=0){
		perror("pthread create error");
	}
	out_stat(&attr);
	void *result;
	if(pthread_join(default_pt,&result)!=0){
		perror("pthread_join error");
	}else{
		printf("return result: %d\n",(int)(intptr_t)result);
	}

	//设置分离属性，以分离属性方式启动子线程
	if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED)!=0){
		//设置线程状态函数就相当于启动子线程函数
		perror("pthread_sttr_setdetachstat error");
	}
	out_stat(&attr);
	//销毁线程属性
	pthread_attr_destroy(&attr);
	return 0;
}
