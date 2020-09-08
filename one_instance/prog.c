#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<errno.h>

#define SEM "semsanbackup1"	//man 7 sem_overview
#define PERM 0755

	int  semid;
	struct sembuf sbuf;

void one_instance_lock()
{

	if((semid = semget(ftok("/home/bhojas", 16457), 1, IPC_CREAT | 0644)) == -1)
	{
		perror("semget");
		exit(1);
	}
	printf("semid:: %d\n", semid);

	sbuf.sem_num = 0;
	sbuf.sem_op  = -1;
	sbuf.sem_flg =  ;
		

	if(-1 == semop(semid, &sbuf, 1))
	{
		perror("semop");
		if(errno == EAGAIN)
		{
			printf("Another instance is already running..\n");
		}
	}
	else
	{
		printf("semop() Success\n");
	}
/*
	if((sem = sem_open(SEM, O_CREAT | O_EXCL, PERM, 1)) == SEM_FAILED)
	{
		perror("sem_open");
		if(errno == EACCES)
			printf("EACCES\n");
		if(errno == EEXIST)
			printf("EEXIST\n");
		if(errno == EINVAL)
			printf("EINVAL\n");
		if(errno == ENOENT)
			printf("ENOENT\n");
		printf("Another Instance is already running..\n");
		exit(1);
	}
	printf("sem: %p\n", sem);

	if(-1 == sem_trywait(sem))
	{
		perror("sem_trywait");
		if(errno == EAGAIN)
		{
			printf("Another Instance of the Daemon is Running..\n");
			exit();
		}
	}
	printf("Lock Acquired..\n");
*/
}

void one_instance_unlock()
{
	printf("Performing Unlocking..\n");

	sbuf.sem_num = 0;
	sbuf.sem_op  = 1;
	sbuf.sem_flg = 0;

	if(-1 == semop(semid, &sbuf, 1))
	{
		perror("semop");
	}

/*
	if(-1 == sem_unlink(SEM))
	{
		perror("sem_unlink");
	}
	else
	{
		printf("Successfully Unlocked..\n");
	}
*/
}

int main()
{
	printf("Calling one_instance()..\n");
	one_instance_lock();
	printf("Pausing..\n");
	pause();
	return 0;	
}
