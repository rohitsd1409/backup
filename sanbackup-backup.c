#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include"ipc.h"

static int wait_flag = 0;	//flag to indicate whether backup-util should wait to receive data from daemon after passing args
				//Eg: when ./sanbackup -l [<timestamp>]
				//    is selected, data must be passed from daemon to backup util

//verify args and pack struct for daemon
void pack_struct(int argc, char* argv[], struct arg* arg_s)
{
	switch(argc)
	{
		case 2:
			printf("2 arguments..\n");
			if(0 == strcmp(argv[1], "-l"))
			{
				arg_s->optns_en   = 1;
				arg_s->optns_type = 'l';
				arg_s->parm_c	  = 0;
				memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
				wait_flag = 1;
				printf("-l option enabled..\n");
			}
			else if(0 == strcmp(argv[1], "-h"))
			{
				usage();
			}
			else
			{
				err("Invalid paramters!!\n", 1);
			}
			break;

		case 3:
			
			printf("3 arguments..\n");
			if(0 == strcmp(argv[1], "-l"))
			{
				if((strlen(argv[2]) < PARAM_SIZE) && (isvalid_timestamp(argv[2])))
				{
					arg_s->optns_en   = 1;
					arg_s->optns_type = 'l';
					arg_s->parm_c	  = 1;
					memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
					strcpy(arg_s->parm_val[0], argv[2]);
					printf("-l <timestamp> option enabled..\n");
					wait_flag = 1;
				}
				else
				{	
					err("Invalid parameters!!\n", 2);
				}
			
			}
			else if(0 == strcmp(argv[1], "-f"))
			{
				if((strlen(argv[2]) < PARAM_SIZE) && (isvalid_frequency(argv[2])))
				{
					arg_s->optns_en   = 1;
					arg_s->optns_type = 'f';
					arg_s->parm_c	  = 1;
					memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
					strcpy(arg_s->parm_val[0], argv[2]);
					printf("-f <nmin> option enabled..\n");
				}
				else
				{	
					err("Invalid parameters..\n", 3);
				}
				
			}
			else if((strcmp(argv[1], "-f") != 0) && (strcmp(argv[1], "-l") != 0))
			{
				//verify that both <src> and <dest> paths are valid
				if((-1 == chdir(argv[1])) || (-1 == chdir(argv[2])))
				{
					err("Atleast one out of the <src> and <dest> paths is invalid..\nPlease make sure that arguements passed are directories and not files!!", 4);
				}
				else if((strcmp(argv[1],".") == 0) || (strcmp(argv[1],"..") == 0) || (strcmp(argv[2],".") == 0) || (strcmp(argv[2],"..") == 0))  
				{
					err("Error: Please enter pathname rather than using \".\" and \"..\"\n", 4);
				}
				else
				{
					printf("Valid <src> and <dest> path..\n");
				}

	
				if((strlen(argv[1]) < PARAM_SIZE) && (strlen(argv[2]) < PARAM_SIZE))
				{
					arg_s->optns_en   = 0;
					arg_s->optns_type = '\0';
					arg_s->parm_c	  = 2;
					memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
					strcpy(arg_s->parm_val[0], argv[1]);
					strcpy(arg_s->parm_val[1], argv[2]);
					printf("sanbackup <src> <dest> enabled..\n");
				}
				else
				{	
					err("Invalid parameters.\n", 5);
				}
			}
			else
			{	
				err("Invalid parameters.\n", 5);
			}
	
			break;

		case 4:
			printf("4 arguements..\n");
			if(0 == strcmp(argv[1], "-r"))
			{
				if((strlen(argv[2]) < PARAM_SIZE) && (strlen(argv[3]) < PARAM_SIZE))
				{
					arg_s->optns_en   = 1;
					arg_s->optns_type = 'r';
					arg_s->parm_c 	  = 2;
					memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
					strcpy(arg_s->parm_val[0], argv[2]);
					strcpy(arg_s->parm_val[1], argv[3]);
					printf("sanbackup -r <file/dir> <newdir> enabled..\n");
				}
				else
				{	
					err("Invalid parameters..\n", 6);
				}
			}
			else
			{	
				err("Invalid parameters..\n", 6);
			}

			break;
		
		case 5:
			printf("5 arguements..\n");
			if(0 == strcmp(argv[1], "-r"))
			{
				if((strlen(argv[2]) < PARAM_SIZE) && (strlen(argv[3]) < PARAM_SIZE) && (strlen(argv[2]) < PARAM_SIZE) && (isvalid_timestamp(argv[4])))
				{
					arg_s->optns_en   = 1;
					arg_s->optns_type = 'r';
					arg_s->parm_c	  = 3;
					memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
					strcpy(arg_s->parm_val[0], argv[2]);
					strcpy(arg_s->parm_val[1], argv[3]);
					strcpy(arg_s->parm_val[2], argv[4]);
					printf("sanbackup -r <file/dir> <newdir> <timestamp> enabled..\n");
				}
				else
				{	
					err("Invalid parameters..\n", 7);
				}
			}
			else if(0 == strcmp(argv[1], "-f"))
			{
				//verify that both <src> and <dest> paths are valid
				if((-1 == chdir(argv[3])) || (-1 == chdir(argv[4])))
				{
					err("Atleast one out of the <src> and <dest> paths is invalid..\n", 4);
				}
				else
				{
					printf("Valid <src> and <dest> path..\n");
				}


				if((isvalid_frequency(argv[2])) && (strlen(argv[3]) < PARAM_SIZE) && (strlen(argv[2]) < PARAM_SIZE))
				{
					arg_s->optns_en   = 1;
					arg_s->optns_type = 'f';
					arg_s->parm_c	  = 3;
					memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
					strcpy(arg_s->parm_val[0], argv[2]);
					strcpy(arg_s->parm_val[1], argv[3]);
					strcpy(arg_s->parm_val[2], argv[4]);
					printf("sanbackup -f <nmin> <src> <dest> enabled..\n");
				}
				else
				{	
					err("Invalid parameters..\n", 8);
				}
			}
			else
			{	
				err("Invalid parameters..\n", 8);
			}

			break;
			
		default:

			err("Invalid parameters!\n", 9);
			usage();
		}

}

//NOTE: 
//	All error checking on supplied arguements is done here
//	and daemon assumes that it is supplied only valid data
int main(int argc, char* argv[])
{
	struct 	arg 	arg_s;		//struct to wrap arguements to be sent to daemon
	int	fd;			//file descriptor for fifo
	char	buf[16];
	int	nbytes;

	//verify and wrap cmdline args into arg struct obj.
	pack_struct(argc, argv, &arg_s);

	printf("\nStruct contents\n\n");
	printf("options enabled: %d\n", arg_s.optns_en);
	printf("options type: %c\n", arg_s.optns_type);
	printf("parameter count: %d\n", arg_s.parm_c);
	printf("parameter[0]: %s\n", arg_s.parm_val[0]);
	printf("parameter[1]: %s\n", arg_s.parm_val[1]);
	printf("parameter[2]: %s\n", arg_s.parm_val[2]);

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
	printf("Sending arg struct(%d bytes) to daemon..\n", sizeof(struct arg));
	if(write(fd, &arg_s, sizeof(struct arg)) != sizeof(struct arg))
	{
		perr("write", 11);
	}
	printf("Data written successfully..\n");	


	if(wait_flag)
	{
		printf("wait_flag set..\n");
	/*	//close fifo
		close(fd);

		//open fifo
		printf("Trying to connect with daemon..\n");
		if((fd = open(FIFO_PATH, O_RDWR)) == -1)
		{
			printf("Failed to connect..\n");
			perr("open", 10);
		}

		printf("fifo opened..(%d)\n", fd);
		printf("Waiting to receive data from daemon..\n");
		*/
		while(1)
		{
			
			printf("Waiting to receive data..\n");
			nbytes = read(fd, buf, sizeof(buf));
			printf("Data received: %d bytes..\n", nbytes);
		
			if(0 == nbytes || (nbytes == 1 || buf[0] == '\0'))
			{
				break;
			}
			else if(-1 == nbytes)
			{
				perr("read", 12);
			}

			if(-1 == write(1, buf, nbytes))
			{
				perr("write", 13);
			}
		}
		printf("Received all data from daemon..\n");
	}
	else
	{
		printf("wait_flag not set..\n");
	}
	
	printf("DONE..\n");

	//close fifo
	close(fd);
	
	return 0;
}
