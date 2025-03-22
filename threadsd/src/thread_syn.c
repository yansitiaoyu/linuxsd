#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
/*
 *一个线程计算存储计算结果，一个获得计算结果，两个线程需要实现同步
 */
//定义存储结果结构体
typedef struct{
	int             ret;     //获得计算结果
	int             is_wait; //等待状态判断位
	pthread_cond_t  cond;    //同步条件变量
	pthread_mutex_t mutex;   //共享资源锁
}Result;

//计算结果线程处理函数
void* set_result(void* arg){
	int sum=0;
	for(int i=1;i<20;i++){
		sum+=i;
	}
	Result *R=(Result*)arg;
	R->ret=sum;
	//is_wait是共享资源，访问时需要上锁
	pthread_mutex_lock(&R->mutex);
	while(!R->is_wait){
		//循环等待的过程中需要释放互斥锁
		pthread_mutex_unlock(&R->mutex);
		usleep(100);
		//等待结束检查is_wait时继续上锁
		pthread_mutex_lock(&R->mutex);
	}
	//退出循环时需要释放锁
	pthread_mutex_unlock(&R->mutex);
	pthread_cond_broadcast(&R->cond);
	return (void*)0;
}
//获得结果线程处理函数
void* get_result(void* arg){
	Result* R=(Result*)arg;
	//get函数检查is_wait的过程中同样需要上锁
	pthread_mutex_lock(&R->mutex);
	if(R->is_wait==0){
		R->is_wait=1;
	}
	//解锁
	//pthread_mutex_unlock(&R->mutex);
	pthread_cond_wait(&R->cond,&R->mutex);
	//解锁再等待之后
	pthread_mutex_unlock(&R->mutex);
	printf("get result:%d\n",R->ret);
	return (void*)0;
}

int main()
{
	//定义线程
	pthread_t p1,p2;
	//定义存储结果结构体
	Result* R;
	//初始化同步条件变量，互斥锁
	pthread_cond_init(&R->cond,NULL);
	pthread_mutex_init(&R->mutex,NULL);
	//创建计算结果线程
	if(pthread_create(&p1,NULL,set_result,(void*)R)!=0){
		perror("pthread_create error");
	}
	if(pthread_create(&p2,NULL,get_result,(void*)R)!=0){
		perror("pthread_create error");
	}
	//运行线程p1,p2
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	//销毁同步条件变量，互斥锁
	pthread_cond_destroy(&R->cond);
	pthread_mutex_destroy(&R->mutex);
	return 0;
}
