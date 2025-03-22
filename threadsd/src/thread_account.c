#include "account.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
/*创建两线程模拟银行卡同时操作*/
//定义线程处理函数传入参数结构体
typedef struct{
	char name[20];
	account *a;
	double mon;
}ArgStu;

void* start_attr(void* arg){
	ArgStu *Arg=(ArgStu*)arg;
	sleep(1);
	double mon=withdraw(Arg->a,Arg->mon);
	printf("%s withdraw %f , 账户余额为%f\n",Arg->name,mon,balance_query(Arg->a));
}

int main(){
	//声明线程id；
	pthread_t p1,p2;
	//创建账户；
	account *a=account_create(123456,10000.0);
	//创建线程传入参数结构体
	ArgStu arg1={"boy",a,10000.0};
	ArgStu arg2={"girl",a,10000.0};
	//创建线程分别传入参数arg1,arg2；
	if(pthread_create(&p1,NULL,start_attr,(void*) &arg1)!=0){
		perror("pthread_create p1 error");
	}
	if(pthread_create(&p2,NULL,start_attr,(void*) &arg2)!=0){
		perror("pthread_creat p2 error");
	}
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	//输出账户余额
	printf("account balance %f\n",a->balance);
	//销毁账户释放内存
	account_destroy(a);
	return 0;
}
