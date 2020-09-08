#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"ipc.h"
#include<fcntl.h>
/*
struct tm 
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};
*/
#define test(ptr) if(ptr == NULL)\
			printf("Exiting..\n");\
			exit(1);
		
int main(int argc, char* argv[])
{
	test(NULL);

/*
	time_t tm_s;
	struct tm* time_s;
	
	tm_s = time(NULL);
	time_s = localtime(&tm_s);	

	printf("Year: %d\n", time_s->tm_year + 1900);
	printf("Month: %d\n", time_s->tm_mon);
	printf("Day: %d\n", time_s->tm_mday);
	printf("Hour: %d\n", time_s->tm_hour);
	printf("Min: %d\n", time_s->tm_min);
	printf("Sec: %d\n", time_s->tm_sec);
	
	printf("%lld\n", atoll(argv[1]));	

	if(-1 == mkdir(argv[1], 0766))
	{
		printf("Failed to create dir..\n");
	}
*/
/*
	int fd;

        //open fifo 
        printf("Trying to connect with daemon..\n"); 
        if((fd = open(FIFO_PATH, O_RDWR)) == -1) 
        { 
                printf("Failed to connect..\n"); 
                perr("open", 10); 
        } 
        printf("fifo opened..(%d)\n", fd); 
        printf("Connected with daemon..\n"); 
 
        //send data 
        printf("Sending data..\n", sizeof(struct arg)); 
        if(5 != write(fd, "Hello", 5)) 
        { 
                perr("write", 11); 
        } 
        printf("Data written successfully..\n");         

	//close(fd);	
        printf("Sending data..\n", sizeof(struct arg)); 
        if(1 != write(fd, "\0", 1)) 
        { 
                perr("write", 11); 
        } 
        printf("Data written successfully..\n");         
*/
	struct stat s;
	stat("makefile", &s);
	struct timespec times[2];
	times[0].tv_sec=0;
	times[0].tv_nsec=s.st_atime;
	times[1].tv_sec=0;
	times[1].tv_nsec=s.st_mtime;
	if(-1 == utimensat(0, "/home/bhojas/sys_pro/backup-util/randomfile", times, AT_SYMLINK_NOFOLLOW))
	{
		perror("utimesat");
	}

	return 0;
}
