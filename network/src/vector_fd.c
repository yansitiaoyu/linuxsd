#include "vector_fd.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <malloc.h>
#include <memory.h>

/*查找fd的下标*/
static int indexof(VectorFD *vfd,int fd)
{
	assert(vfd!=NULL);
	int i=0;
	/*while(i<vfd->counter){
		if(vfd->fd[i]!=fd){
			i++;
		}else{
			return i;
		}
	}
	if(i==vfd->counter){
		return -1;
	}*/
	for(;i<vfd->counter;i++){
		if(vfd->fd[i]==fd){
			return i;
		}
	}
	return -1;
} 

/*容器扩展*/
static void encapacity(VectorFD* vfd)
{
	if(vfd->counter >= vfd->capacity){
		int *fdv=(int*)calloc(vfd->counter+5,sizeof(int));
		assert(fdv!=NULL);
		memcpy(fdv,vfd->fd,sizeof(int)*vfd->counter);
		free(vfd->fd);
		vfd->fd=fdv;
		vfd->capacity += 5;
	}
}

/*创建容器*/
VectorFD *create_vector_fd()
{
	VectorFD *vfd=(VectorFD*)calloc(1,sizeof(VectorFD));
	assert(vfd!=NULL);
	vfd->fd=(int*)calloc(5,sizeof(int));
	assert(vfd->fd!=NULL);
	vfd->counter=0;
	vfd->capacity=0;
	return vfd;
}
/*销毁容器*/ 
void destroy_vector_fd(VectorFD *vfd)
{
	assert(vfd!=NULL);
	free(vfd->fd);
	free(vfd);
}
/*读取下标指定元素*/
int get_fd(VectorFD *vfd,int index)
{
	assert(vfd!=NULL);
	if(index<0 || index >= vfd->counter){
		return -1;
	}
	return vfd->fd[index];
}
/*删除指定下标元素*/
void delete_fd(VectorFD *vfd,int fd)
{
	assert(vfd!=NULL);
	int index=indexof(vfd,fd);
	if(index==-1)return;
	for(int i=index;i<vfd->counter;i++){
		vfd->fd[i]=vfd->fd[i+1];
	}
	vfd->counter--;
}
/*添加套接字描述符*/
void add_fd(VectorFD *vfd,int fd)
{
	assert(vfd!=NULL);
	encapacity(vfd);
	vfd->fd[vfd->counter++]=fd;
}

