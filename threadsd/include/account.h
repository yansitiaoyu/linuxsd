#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__
#include <pthread.h>
typedef struct {
	int idcard;
	double balance;
	//定义所，一般一个共享资源对应一把所，而不是定义一个全局锁
	pthread_mutex_t mutex;
}account;

//创建账户；
//extern account *account_create(int idcard,double balance,pthread_mutex_t mutex);
//创建结构体实例时，所是操作系统管理的，不需要自己传入实参
extern account *account_create(int idcard,double balance);

//取款
extern double withdraw(account *a,double mon);

//存钱
extern double deposit(account *a,double mon);

//查询余额

extern double balance_query(account *a);

//销毁账户
extern void account_destroy(account* a);

#endif
