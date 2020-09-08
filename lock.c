#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include"lock.h"
#include<errno.h>

//Check Whether Current Instance of the Daemon is the only one Running or not
//args:
//	none
//returns:
//	0: success (this is the only instance of Daemon)
//	-1: Another Instance of the Daemon is already Running
//(Pitfall: Some evil prog. may acquire lock on this file and hence, our daemon instance won't run
//thinking that some other instance of daemon is already running) 

int single_instance_check()
{
	int fd;
	struct flock lock;

	printf("single_instance_check() called..\n");
	printf("Opening lock-file..\n");
	if((fd = open(DUMMY_FILENAME, O_CREAT | O_WRONLY, 0644)) == -1)
	{                                       //file flags must match with type of lock acquire eg: WR flag is write lock is acquired
						//otherwise, BAD file descriptor error is thrown by fcntl
		perror("open");
		exit(1);
	}


	printf("Configuring Lock..\n");
	lock.l_type 	= F_WRLCK;
	lock.l_whence 	= SEEK_SET;
	lock.l_start 	= 0;
	lock.l_len 	= 0;
	printf("Lock Configured..\n");

	printf("Acquiring Lock..\n");
	if(-1 == fcntl(fd, F_SETLK, &lock))
	{
		perror("fcntl");
		if(errno == EACCES || errno == EAGAIN)
		{
			printf("Probably, another instance of daemon is running..\n");
			return -1;
		}
		exit(1);
	}
	printf("Lock Acquired..\n");
	return 0;
}
