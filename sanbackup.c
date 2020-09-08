#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<errno.h>
#include"ipc.h"
#include"sec.h"


static short wait_flag = 0;	//flag to indicate whether backup-util should wait to receive data from daemon after passing args
				//Eg: when ./sanbackup -l [<timestamp>]
				//    is selected, data must be passed from daemon to backup util

static short newbackup_flag = 0; //flag to indicate whether user has requested for a new backup operation


/////////////////////////////////////////////////////////////////////////////////
//verify args and pack struct for daemon
//args:
//	argc: Num. of args supplied to this util by user at the runtime
//	argv: arguement vector containing args supplied by user at the runtime
//	arg_s: struct to be packed with data by this function
//
//returns:
//	nothing
/////////////////////////////////////////////////////////////////////////////////
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
				exit(1);
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
				if((strlen(argv[2]) < FREQ_LEN) && (isvalid_frequency(argv[2])))
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
				//path length is within permissible limits
				if(!((strlen(argv[1]) < PARAM_SIZE) && (strlen(argv[2]) < PARAM_SIZE)))
				{
					err("Path length too long..\n", 1);
				}
				//path is valid
				if((-1 == chdir(argv[1])) || (-1 == chdir(argv[2])))
				{
					err("Atleast one out of the <src> and <dest> paths is invalid..\nPlease make sure that arguements passed are directories and not files!!", 4);
				}
				//Abs. path is supplied rather than "." or ".."
				else if((strcmp(argv[1],".") == 0) || (strcmp(argv[1],"..") == 0) || (strcmp(argv[2],".") == 0) || (strcmp(argv[2],"..") == 0)) 
				{
					err("Error: Please enter pathname rather than using \".\" and \"..\"\n", 4);
				}
				else
				{
					printf("Valid <src> and <dest> path..\n");
					newbackup_flag	  = 1;
					arg_s->optns_en   = 0;
					arg_s->optns_type = '\0';
					arg_s->parm_c	  = 2;
					memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
					strcpy(arg_s->parm_val[0], argv[1]);
					strcpy(arg_s->parm_val[1], argv[2]);
					
					printf("sanbackup <src> <dest> enabled..\n");
				}
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
			else if(isvalid_ip(argv[3]))	//$./sanbackup <src> <dest> <ip>
			{
				printf("%s is a valid IP address Representation..\n", argv[3]);

				//verify that both <src> and <dest> paths are valid
				//path length is within permissible limits
				if(!((strlen(argv[1]) < PARAM_SIZE) && (strlen(argv[2]) < PARAM_SIZE)))
				{
					err("Path length too long..\n", 1);
				}
				//path is valid
				if((-1 == chdir(argv[1])) || (-1 == chdir(argv[2])))
				{
					err("Atleast one out of the <src> and <dest> paths is invalid..\nPlease make sure that arguements passed are directories and not files!!", 4);
				}
				//Abs. path is supplied rather than "." or ".."
				else if((strcmp(argv[1],".") == 0) || (strcmp(argv[1],"..") == 0) || (strcmp(argv[2],".") == 0) || (strcmp(argv[2],"..") == 0)) 
				{
					err("Error: Please enter pathname rather than using \".\" and \"..\"\n", 4);
				}
				else
				{
					printf("Valid <src> and <dest> path..\n");
					newbackup_flag	  = 1;
					arg_s->optns_en   = 0;
					arg_s->optns_type = '\0';
					arg_s->parm_c	  = 2;
					memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
					strcpy(arg_s->parm_val[0], argv[1]);
					strcpy(arg_s->parm_val[1], argv[2]);
					strcpy(arg_s->dest_ip,     argv[3]);
					printf("sanbackup <src> <dest> <ipaddress> enabled..\n");
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
				if((strlen(argv[2]) < PARAM_SIZE) && (strlen(argv[3]) < PARAM_SIZE)  && (isvalid_timestamp(argv[4])))
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
				//verify that both <src> and <dest> paths are valid and frequency is valid
				if(!((strlen(argv[2]) < FREQ_LEN) && (strlen(argv[3]) < PARAM_SIZE) && (strlen(argv[4]) < PARAM_SIZE)))
				{
					err("Invalid length frequency or path..\n", 1);	
				}
			
				if((-1 == chdir(argv[3])) || (-1 == chdir(argv[4])))
				{
					err("Atleast one out of the <src> and <dest> paths is invalid..\n", 4);
				}

				//Abs. path is supplied rather than "." or ".."
				else if((strcmp(argv[3],".") == 0) || (strcmp(argv[3],"..") == 0) || (strcmp(argv[4],".") == 0) || (strcmp(argv[4],"..") == 0)) 
				{
					err("Error: Please enter pathname rather than using \".\" and \"..\"\n", 4);
				}
				printf("Valid <src> and <dest> path..\n");


				if(isvalid_frequency(argv[2]))
				{
					newbackup_flag	  = 1;
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

		case 6:
			if(0 == strcmp(argv[1], "-f"))	//$./sanbackup -f  <src> <dest> <ip-address>
			{
				//verify that
				//	* both <src> and <dest> paths are valid 
				//	* frequency is valid
				//	* IP address is valid
				if(!((strlen(argv[2]) <= FREQ_LEN) && (strlen(argv[3]) < PARAM_SIZE) && (strlen(argv[4]) < PARAM_SIZE)))
				{
					err("Invalid length frequency or path..\n", 1);	
				}
			
				if((-1 == chdir(argv[3])) || (-1 == chdir(argv[4])))
				{
					err("Atleast one out of the <src> and <dest> paths is invalid..\n", 4);
				}
				printf("Valid <src> and <dest> path..\n");


				if(!isvalid_frequency(argv[2]))
				{
					err("Invalid Value of Frequency...\n", 1);
				}
				printf("Valid Frequency..\n");

				if(!isvalid_ip(argv[5]))
				{
					err("Invalid IP address...\n", 1);
				}
				printf("Valid IP address...\n");
				
				newbackup_flag	  = 1;
				arg_s->optns_en   = 1;
				arg_s->optns_type = 'f';
				arg_s->parm_c	  = 3;
				memset(arg_s->parm_val, 0,  MAX_PARAM * PARAM_SIZE);
				strcpy(arg_s->parm_val[0], argv[2]);
				strcpy(arg_s->parm_val[1], argv[3]);
				strcpy(arg_s->parm_val[2], argv[4]);
				strcpy(arg_s->dest_ip,     argv[5]);
				printf("sanbackup -f <nmin> <src> <dest> <ip-address> enabled..\n");
			}

			break;
			
		default:

			err("Invalid parameters!\n", 9);
			usage();
			exit(1);
		}

}


/////////////////////////////////////////////////////////////////////////////
//fetch Authentication key from user and generate hash associated with it
//args:
//	none
//returns:
//	hash:	character string associated with the input key
/////////////////////////////////////////////////////////////////////////////
char* get_hash()
{
	char* key;
	char* hash;

	//get authentication key from user
	key  = get_key();
	//generate hash associated with the key
	hash = generate_hash(key);
	printf("Hash: %s\n", hash);

	free(key);

	return hash;
}




//NOTE: 
//	All error checking on supplied arguements is done here
//	and daemon assumes that it is supplied only valid data
int main(int argc, char* argv[])
{
	struct arg* 		arg_s;		//struct to wrap arguements to be sent to daemon
	char*			buf;		//to store data sent by daemon
	int			nbytes;
	int 			sock_fd;	//socket descriptor
	struct sockaddr_in 	daemon;		//Socket details for connection with Peer Running Daemon 
	

	//Verify that this util is being run with root priviliges
	if(0 != getuid())
	{
		printf("Operation Not Permitted..\n");
		exit(1);
	}

	//allocate reqd. memory
	arg_s = (struct arg*)calloc(1, sizeof(struct arg));
	if(arg_s == NULL)
	{
		exit(1);
	}

	buf = (char*)calloc(BUF_SIZE, sizeof(char));
	if(buf == NULL)
	{
		free(arg_s);
		exit(1);
	}


	//verify and wrap cmdline args into arg struct obj.
	pack_struct(argc, argv, arg_s);

	//for each new backup request
	if(1 == newbackup_flag)
	{
		//generate Hash		
		char* hash;
		hash = get_hash();
		printf("Request for a new backup operation..\n");
		strcpy(arg_s->hash, hash);	
		free(hash);

		//supply default ip if not entered by user
		if(0 == strcmp(arg_s->dest_ip, ""))
		{
			printf("IP not supplied..Using default ip..\n");
			strcpy(arg_s->dest_ip, LH_IPv4); 
		}
	}


	printf("Size of struct: 	%d\n", sizeof(struct arg));

	printf("\nStruct contents\n\n");
	printf("options enabled: 	%d\n", arg_s->optns_en);
	printf("options type: 		%c\n", arg_s->optns_type);
	printf("parameter count: 	%d\n", arg_s->parm_c);
	printf("parameter[0]: 		%s\n", arg_s->parm_val[0]);
	printf("parameter[1]: 		%s\n", arg_s->parm_val[1]);
	printf("parameter[2]: 		%s\n", arg_s->parm_val[2]);
	printf("dest. ip:    		%s\n", arg_s->dest_ip);
	printf("Hash: 			%s\n", arg_s->hash);



	//create socket
	printf("Creating Socket..\n");
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		free(arg_s);
		free(buf);
		perr("socket", 1);
	}
	printf("Socket Created: %d..\n", sock_fd);

	//Configure Connection Details
	printf("Configuring Connnection Details..\n");
	daemon.sin_family	=	AF_INET;
	daemon.sin_port		=	htons(PORT_LH);
	if(-1 == inet_pton(AF_INET, LH_IPv4, &daemon.sin_addr))
	{
		perror("inet_pton");
		printf("Closing Socket\n");
		free(arg_s);
		free(buf);
		if(-1 == close(sock_fd))
		{
			perror("close");
		}
		else
		{
			printf("Socket Closed..\n");
		}
		return 0;
	}
	printf("Connnection Details Configured Successfully..\n");

	//Connect With Daemon
	printf("Trying to Connect with Daemon..\n");
	if(-1 == connect(sock_fd, (struct sockaddr*) &daemon, sizeof(struct sockaddr_in)))
	{
		perror("connect");
		printf("Closing Socket\n");
		free(arg_s);
		free(buf);
		if(-1 == close(sock_fd))
		{
			perror("close");
		}
		else
		{
			printf("Socket Closed..\n");
		}
		return 0;
	}
	printf("Connection Established with Daemon..\n");



	//send Data to Daemon
	printf("Sending arg struct (%d bytes) to daemon..\n", sizeof(struct arg));
	while(sizeof(struct arg) != send(sock_fd, arg_s, sizeof(struct arg), 0))
	{
		perror("send");
		if(errno == EINTR)
		{	continue;
		}
		printf("Failed to Write Data Successfully..\n");
		printf("Closing Socket\n");
		free(arg_s);
		free(buf);
		if(-1 == close(sock_fd))
		{
			perror("close");
		}
		else
		{
			printf("Socket Closed..\n");
		}
		return 0;
	}
	printf("Data Written Successfully..\n");


	//////////////////////////////////
	//read data from daemon (if reqd.)
	//Eg: in case of $./sanbackup -l
	//////////////////////////////////
	if(wait_flag)
	{
		printf("wait_flag set..\n");
		printf("Waiting to receive data..\n");
		while((nbytes = recv(sock_fd, buf, sizeof(buf), 0)))
		{
			if(nbytes == -1)
			{
				perror("recv");
				if(errno == EINTR)
				{
					continue;
				}
				printf("Closing Socket\n");
				free(arg_s);
				free(buf);
				if(-1 == close(sock_fd))
				{
					perror("close");
				}
				else
				{
					printf("Socket Closed..\n");
				}
				break;
			}
			else
			{
				printf("%s", buf);
			}
		}
	}

	printf("Closing Socket\n");
	if(-1 == close(sock_fd))
	{
		free(arg_s);
		free(buf);
		perror("close");
	}
	else
	{
		printf("Socket Closed..\n");
	}



	//free memory
	free(arg_s);
	free(buf);
	
	return 0;
}
