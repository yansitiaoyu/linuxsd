#include "account.h"
#include "semnums_pv.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

//取款
double withdraw(account *a,double mon){
	assert(a!=NULL);
	//P操作
	P(a->semid,0,1);
	if(a->balance<0 || a->balance<mon){
		return 0.0;
		//错误退出也要V操作
		V(a->semid,0,1);
		return 0.0;
	}
		double balance=a->balance;
		balance -= mon;
		a->balance=balance;
		V(a->semid,0,1);
		return mon;
}
//存款
double deposit(account *a,double mon){
	assert(a!=NULL);
	//P操作
	P(a->semid,0,1);
	if(mon<0){
		return 0.0;
		//错误退出时V操作
		V(a->semid,0,1);
		return 0.0;
	}
		double balance=a->balance;
		balance +=mon;
		a->balance=balance;
		//V操作
		V(a->semid,0,1);
		return mon;
}

//查询余额
double balance_query(account *a){
	assert(a!=NULL);
	//P操作
	P(a->semid,0,1);
	return a->balance;
	V(a->semid,0,1);
}

