#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

typedef struct {
	int idcard;
	double balance;
	int semid;
}account;

//取款
extern double withdraw(account *a,double mon);

//存钱
extern double deposit(account *a,double mon);

//查询余额

extern double balance_query(account *a);

#endif
