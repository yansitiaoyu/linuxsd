#include "io.h"
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

# define BUFFER_LEN 1024
void copy(int fdin,int fdout)
{
	char buffer[BUFFER_LEN];
	ssize_t size;
	while(size=read(fdin,buffer,BUFFER_LEN)>0){
		if(write(fdout,buffer,size)!=size){
			fprintf(stderr,"write error:%s",strerror(errno));
			exit(1);
		}
	}
	if(size<0){
		fprintf(stderr,"read error:%s",strerror(errno));
		exit(1);
	}
}
