#ifndef IPC_H
#define IPC_H

#include"sec.h"						//retrieve hash len 


#define err(str, status)  printf(str);usage();exit(status);
#define perr(str, status) perror(#str);exit(status);

#define  PARAM_SIZE    	1024				//max size of each arg. obtained from cmdline 
							//(Including '\0' i.e. if PARAM_SIZE is 5, 
							//then, "abcde" is invalid(6 chars) but "abcd" is a valid string(5 chars including '\0')

#define  MAX_PARAM      3				//max no. parameters that are allowed (this count doesnot include ip address and hash) 
#define  FREQ_LEN	10				//Including '\0' i.e. Max Frequency = "999999999" (which ofcourse is impractical to set for a backup)
#define  BUF_SIZE	1024				//Buffer Size, Used while reading and writing of data
#define  IP_LEN 	129				//Len. of ip address Including '\0' (128 for IPv6 compatibility in future + 1 (for '\0'))
#define  PORT_LH	5000				//PORT for communication on LOCAL HOST
#define  PORT_RH	5001				//PORT for Communication of Remote HOST
#define  LH_IPv4  	"127.0.0.1"			//Local Ipv4 Address
#define  CON_BACKLOG	5				//Connection Backlog for listen()


///////////////////////////////////////////////////////////////////////////////////////////
//struct to wrap arguements passed to backup-util
//this struct is sent to daemon to perform appropriate operation as per arguements
//Eg:
//      $./backup-util -f 100 ~/Documents /media/user/Local_Disk
//	Authentication Key: xxxxx
//      will be stored as:
//
//      optns_en        = 1     //options are enabled (-f supplied)
//      optns_type      = 'f'   //supplied option
//      parm_c          = 2     //2 parameters supplied for option 'f' (src and destination paths)
//      &parm_val[0]    = "~/Documents"
//      &parm_val[1]    = "/media/user/Local_Disk"
//      &parm_val[2]    = NULL
//	dest_ip[]	= 127.0.0.1 (default)
//	hash		= <hash value>
//////////////////////////////////////////////////////////////////////////////////////////////
struct arg 
{
        short 	optns_en;         			//is any option enabled (-f, -l or -r) ??
        char  	optns_type;       			//if enabled, which option out of the valid one's is enabled('f' or 'l' or 'r')??
        short 	parm_c;           			//parameters count (no. of parameters passed along with particular option)      
        char  	parm_val[MAX_PARAM][PARAM_SIZE];  	//max. possible parameters are 3 
							//excluding ip address
	char	dest_ip[IP_LEN];			//Ip Address of host where files are to be backed
	char 	hash[HASH_LEN];				//Store hash associated with backup's password
							//for authentication purposes
	
};



void usage();				//print util usage instructions
int isvalid_frequency(char* ptr);	//Check whether the supplied value of backup frequency is valid or not
int isvalid_timestamp(char* ptr);	//Check whether the supplied value of timestamp is valid or not
int isvalid_ip(char* ptr);		//Check whether the supplied value of IP address is valid or not





#endif
