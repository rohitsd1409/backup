#ifndef BACKUP_H
#define BACKUP_H

#include<sys/time.h>			//struct itimerval
#include<sys/types.h>
#include"ipc.h"

//#define  BACKUP_DIR     "/tmp/backup"			//directory where backup related files will be stored by daemon
//#define  LOGS_DIR	"/tmp/backup/logs"		//dir where logfiles will be stored by daemon
#define  KEY_PATHNAME   "/root"				//pathname used to generate key(ftok())
#define  KEY_PROJID     1001				//proj_id used to generate key(ftok())
#define  MAX_CHILD	100				//Upper limit on number of backup operations allowed by daemon
							// Eg: backup src1 dest1
							//	backupd src2 dest2  count as 2 backup operations specified by user.
							//	in this manner, user can specify backups only upto MAX_CHILD limit
#define  BD_FILENAME   	"/tmp/backup/backupdetails"	//Filename where info. related to every backup done by daemon is stored 
							//i.e. it stores backup records
#define  TIMESTAMP_LEN  15				//length of timestamp
							//format of timestamp is: YYYYMMDDHHMMSS
							//Including '\0'
#define  FILENAME_LEN 	256				//filename can be of max 255 chars + 1 (for nul char)

#define  BUFSIZE	512				//size used when backing up file from src. to dest	
#define  NBACKUPS	7				//keep record of atmost how many backups per backup operation
#define  DFLT_BACKUP_INTERVAL 10			//default val. of backup interval in minutes
//#define  DIRMODE	0755				//permissions to be used while creating while creating directories


//struct to keep record of a backup operation
struct backup_op
{
	char src[PARAM_SIZE+1];			//source (what to backup?)
						//+1 is used to append "/" at the end of path, if path already doesnot contain "/" at the end
						//Eg:	/home/user/Documents + /
						//			    ^
						//			    |---- user didn't explicitly appended "/" at the end,
						//				  so, we ourselves append "/" at the end when reqd.
						//+1 helps to append "/" in worst case:
						//	let PARAM_SIZE = 5
						//	in this case: /bin\0 is a valid path and of valid length
						//	 but it is dir and we need to append '/' at the end
						//	Hence, string will become:
						//	(of length) PARAM_SIZE + 1
						//		i.e. /bin/ + '\0' 	

	char dest[PARAM_SIZE+1];		//destination (where to backup?)
						//+1 is used to append "/" at the end of path, if path already doesnot contain "/" at the end
						//Eg:	/home/media/user/Disk/mybackups + /
						//			    ^
						//			    |---- user didn't explicitly appended "/" at the end,
						//				  so, we ourselves append "/" at the end when reqd.
	int nmin;			//backup interval
	short int b_ctr;		//backup counter(no. of backups performed for this backup_op)
	char last_bt[NBACKUPS][TIMESTAMP_LEN];	//Keep record of last n backup timestamps atmost
							//(+1 for nul char)
							
	struct itimerval itimer;	//interval timer
	char hash[HASH_LEN];		//store hash of the key used to authenticate operations on this backup task 
	char dest_ip[IP_LEN];		//IP of the host where backup is to be performed
};

//struct to store stat. of each file backed up during a backup operation
struct file_stat
{
	char fs_name[FILENAME_LEN];	//name of file 
	time_t fs_mtime;		//time of last modification
	time_t fs_ctime;		//time of last status change
};

//file stats sent from daemon to remote daemon for each file to be backed up
struct backup_file_stat
{
	char abs_dest[PARAM_SIZE];//abs. path of destination where file is to be backed u
	mode_t mode;	//protection
	uid_t uid;	//user ID of owner
	gid_t gid;	//group ID of owner
	off_t size;	//total size, in bytes
	time_t atime;	//time of last access 
	time_t mtime;	//time of last modification
};






#endif
