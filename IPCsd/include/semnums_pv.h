#ifndef __semnums_pv__
#define __semnums_pv__

//创建并初始化信号集
extern int I(int semnums,int value);

//信号集P操作
extern void P(int semid,int semnum,int value);

//信号集V操作
extern void V(int semid,int semnum,int value);

//信号集销毁
extern void D(int semid);

#endif
