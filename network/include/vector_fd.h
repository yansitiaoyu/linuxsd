#ifndef __VECTOR_FD__
#define __VECTOR_FD__

typedef struct{
	int *fd;
	int counter;
	int capacity;
}VectorFD;

/*创建容器*/
extern VectorFD *create_vector_fd();
/*销毁容器*/
extern void destroy_vector_fd(VectorFD *);
/*读取下标指定元素*/
extern int get_fd(VectorFD *,int index);
/*删除指定下标元素*/
extern void delete_fd(VectorFD *,int fd);
/*添加套接字描述符*/
extern void add_fd(VectorFD *,int fd);
#endif
