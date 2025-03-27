#ifndef __PIPE_SHM_SYN__
#define __PIPE_SHM_SYN__

//初始化管道
extern void init();
//等待管道消息
extern void wait_pipe();
//向管道发送消息
extern void notify_pipe();
//关闭管道
extern void destroy_pipe();

#endif
