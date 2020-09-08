#ifndef BACKUP_ID_H
#define BACKUP_ID_H

#include"ipc.h"

//
//Node to Uniquely identify each backup operation
//As, of now:
//	<src,dest,ip> 	uniquely identify each process
struct backup_id
{
	char 	src[PARAM_SIZE];	//absolute path of src dir.	
	char 	dest[PARAM_SIZE];	//absolute path of dest dir.
	char 	dest_ip[IP_LEN];		//ip_address of machine where data is to be backed up
	pid_t 	pid;			//process id of the child process that is handling this backup operation 	
	struct backup_id* next;		//next node in the list
	struct backup_id* prev;		//prev node in the list
};


struct  backup_id* getnode(pthread_mutex_t* mutex);						//allocate m/y for a node and return it
int insert_node(struct backup_id** head, struct backup_id* node, pthread_mutex_t* mutex);				//insert a node in the backup id list 
int delete_node(struct backup_id** head, struct backup_id* node, pthread_mutex_t* mutex);				//delete a node from the backup id list
struct backup_id* search_backup_node(struct backup_id** head, struct backup_id* node, pthread_mutex_t* mutex);			//search a node in the list

#endif
