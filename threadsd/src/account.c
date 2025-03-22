#include "account.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
//创建账户
account *account_create(int idcard,double balance){
	account *a=(account*)malloc(sizeof(account));
	assert(a != NULL);
	a->idcard=idcard;
	a->balance=balance;
	//a->mutex=mutex;
	//初始化共享所
	pthread_mutex_init(&a->mutex,NULL);//对于所，结构体实例化时直接初始化即可，
	                                   //不需要被当作结构体属性赋值
	return a;
}
//取款
/*double withdraw(account *a,double mon){//这样的代码缺少健壮性；
	double balance=a->balance;
	balance -= mon;
	a->balance=balance;
	return mon;
}*/

//取款修改后的代码
double withdraw(account *a,double mon){
	assert(a!=NULL);
	//上锁
	pthread_mutex_lock(&a->mutex);
	if(a->balance<0 || a->balance<mon){
		printf("withdraw error\n");
		//错误退出也要解所
		pthread_mutex_unlock(&a->mutex);//这条语法的执行顺序为先->再&（先指针再取地址）
		return 0.0;
	}
		double balance=a->balance;
		balance -= mon;
		a->balance=balance;
		pthread_mutex_unlock(&a->mutex);
		return mon;
}
//存钱
double deposit(account *a,double mon){
	assert(a!=NULL);
	//上锁
	pthread_mutex_lock(&a->mutex);
	if(mon<0){
		printf("desposit error\n");
		//错误退出时解锁
		pthread_mutex_unlock(&a->mutex);
		return 0.0;
	}
		double balance=a->balance;
		balance +=mon;
		a->balance=balance;
		//解锁
		pthread_mutex_unlock(&a->mutex);
		return mon;
	
}

//查询余额
double balance_query(account *a){
	return a->balance;
}

//销毁账户
void account_destroy(account* a){
	assert(a!=NULL);
	//销毁所
	pthread_mutex_destroy(&a->mutex);
	free(a);
}

