#ifndef __MSG_H__
#define __MSG_H__
#include <stddef.h>
/*
 *自定义协议体
 */

typedef struct{
	char head[12];//协议头部
	char checknum;//校验码
	char buff[512];//数据
}Msg;

//发动一个自定义数据
extern int write_msg(int sockfd,char *buff,size_t len);

//读取一个自定义数据
extern int read_msg(int sockfd,char *buff,size_t len);

#endif
