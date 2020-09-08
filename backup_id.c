#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include"backup_id.h"



//Search a node in the list from insert_node() and delete_node() etc
//Mutex locking is performed by the fn. that calls this fn.
//Hence, no need to perform lokcing here
//Note:
//	* This fn. must be called only after list mutex has been acquired
//args:
//	head:	head of the list
//	node:	node to be inserted
//	mutex:	Used to Synchronise the insertion
//returns:
//	address of the node if found
//	NULL : if not found	
static struct backup_id* search_node(struct backup_id** head, struct backup_id* node)
{
	struct backup_id* cur;	//current node
	cur = *head;

	printf("search_node() called..\n");
	printf("Serching node: <%s,%s,%s,%d>\n", node->src, node->dest, node->dest_ip, node->pid);
	if(*head == NULL)
	{
		printf("Empty list..No searching performed..\n");
		return NULL;
	}
	do
	{
		printf("Current node: <%s,%s,%s,%d>\n", cur->src, cur->dest, cur->dest_ip, cur->pid);
		//search is successfull:
		//if,
		//	1) <src,dest,ip> matches of 2 nodes
		//	or
		//	2) process id's stored in 2 nodes match but are non-zero
		if(((strcmp(cur->src,  node->src)  == 0) && (strcmp(cur->dest, node->dest) == 0) && (strcmp(cur->dest_ip,   node->dest_ip)   == 0)) ||
		   ((cur->pid == node->pid) && (cur->pid != 0)))
		{
			printf("Node found in the list..\n");
			return cur;
		}

		cur = cur->next;
	}
	while(cur != (*head));

	printf("Node not found in the list..\n");
	return NULL;
}
//acts as interface to other files, so that list can be searched after locking it down
struct backup_id* search_backup_node(struct backup_id** head, struct backup_id* node, pthread_mutex_t* mutex)
{
	struct backup_id* res;	//result
	pthread_mutex_lock(mutex);
	res = search_node(head, node);	
	pthread_mutex_unlock(mutex);
	return res;
}


//Allocate memory for a new node
//args:
//	mutex: Used to synchronise allocation of memory
//returns:
//	address of dynamic m/y allocated for a node
struct backup_id* getnode(pthread_mutex_t* mutex)
{
	struct backup_id* node;

	pthread_mutex_lock(mutex);
	node = (struct backup_id*)calloc(1, sizeof(struct backup_id));
	pthread_mutex_unlock(mutex);

	return node;
}

//Insert a new node into the list
//args:
//	head:	head of the list
//	node:	node to be inserted
//	mutex:	Used to Synchronise the insertion
//returns:
//	 0:	node inserted successfully	
//	-1:	node already exists 
int insert_node(struct backup_id** head, struct backup_id* node, pthread_mutex_t* mutex)
{
	pthread_mutex_lock(mutex);

	printf("Inserting node: <%s,%s,%s,%d>\n", node->src, node->dest, node->dest_ip, node->pid);
	//list is empty
	if(*head == NULL)
	{
		printf("List is empty...\n");
		printf("Node will be inserted as the first elem\n");
		*head = node;
		node->next = *head;
		node->prev = *head;
	}
	else
	{
		//check whether node already doesnot exists in the list
		if(NULL == search_node(head, node))
		{
			printf("Inserting node at the front of the list\n");
			node->next = *head;
			node->prev = (*head)->prev;
			(*head) = node;
			node->next->prev = *head;
			node->prev->next = *head;
		}
		else
		{
			printf("node already exists in the list\n");
			printf("Skipping its insertion..\n");
			pthread_mutex_unlock(mutex);
			return -1;
		}

	}
	pthread_mutex_unlock(mutex);
	return 0;
}

//Delete a node from the list
//args:
//	head:	head of the list
//	node:	node to be inserted
//	mutex:	Used to Synchronise the insertion
//returns:
//	 0:	node deleted successfully	
//	-1:	node doesnot exists 
int delete_node(struct backup_id** head, struct backup_id* node, pthread_mutex_t* mutex)
{
	struct backup_id* n; 

	pthread_mutex_lock(mutex);
	printf("Deleting node: <%s,%s,%s,%d>\n", node->src, node->dest, node->dest_ip, node->pid);

	//node doesnot exists in the list
	if((n = search_node(head, node)) == NULL) 
	{
		printf("node to be deleted doesnot exists in the list..\n");
		pthread_mutex_unlock(mutex);
		return -1;
	}
	//node found
	else
	{
		//case: only one node
		if(n->next == n)
		{
			printf("Deleting node: node is the only element of the list..\n");
			*head = NULL;
		}
		else	//case: more than one node
		{
			n->next->prev = n->prev;
			n->prev->next = n->next;
			if(n == *head)		//node to be deleted is also the head of the list
			{
				printf("deleting node: node is the head of the list..\n");
				*head = n->next;	//update the head
			}
			else
			{
				printf("deleting node: node is a part of the list..\n");
			}
		}
		free(n);
		pthread_mutex_unlock(mutex);
		return 0;
	}
}

/*
//Atleast one element should be present in the list
static void traverse_list(struct backup_id** head)
{
	struct backup_id* cur;

	printf("Traversing list..\n");
	cur = *head;
	if(*head == NULL)
	{
		printf("empty list..\n");
	}
	else
	{
		do
		{
			printf("%s:%d\n", cur->src,cur->pid);
			cur = cur->next;
		}
		while(cur != *head);
	}
	printf("\n\n");
}
*/

/*
void* thread1(void* arg)
{
	//int i = 0;
	//int j = 0;
	struct backup_id* node;
	printf("Thread1 Routine running..\n");

		node = getnode(&backup_id_list.mutex);
		sprintf(node->src,"Thread1: Node:1");
		insert_node(&backup_id_list.head, node, &backup_id_list.mutex);
		delete_node(&backup_id_list.head, node, &backup_id_list.mutex);

		traverse_list(&backup_id_list.head);

		struct backup_id* node2;
		struct backup_id* node3;
		struct backup_id* node4;
		node2 = getnode(&backup_id_list.mutex);
		sprintf(node2->src,"Thread1: Node:2");
		insert_node(&backup_id_list.head, node2, &backup_id_list.mutex);
		node3 = getnode(&backup_id_list.mutex);
		sprintf(node3->src,"Thread1: Node:3");
		insert_node(&backup_id_list.head, node3, &backup_id_list.mutex);
		delete_node(&backup_id_list.head, node3, &backup_id_list.mutex);

		traverse_list(&backup_id_list.head);

		node4 = getnode(&backup_id_list.mutex);
		sprintf(node4->src,"Thread1: Node:4");
		delete_node(&backup_id_list.head, node4, &backup_id_list.mutex);

		traverse_list(&backup_id_list.head);

		struct backup_id* node5;
		node5 = getnode(&backup_id_list.mutex);
		node5->pid = 1024;
		insert_node(&backup_id_list.head, node5, &backup_id_list.mutex);

		struct backup_id* node6;
		node6 = getnode(&backup_id_list.mutex);
		node6->pid = 1024;
		insert_node(&backup_id_list.head, node6, &backup_id_list.mutex);
		delete_node(&backup_id_list.head, node6, &backup_id_list.mutex);

		return NULL;
}



void* thread2(void* arg)
{
//	int i = 0;
//	struct backup_id* node;
	printf("Thread2 Routine running..\n");

	return NULL;
}



int main()
{
	pthread_t t1;	
	pthread_t t2;	
	pthread_create(&t1, NULL, thread1, NULL); 
	pthread_create(&t2, NULL, thread2, NULL); 
	//printf("head: %p\n", backup_id_list.head);
	//backup_id_list.head = getnode(&backup_id_list.mutex);
	//printf("head: %p\n", backup_id_list.head);
	pthread_join(t1, NULL);
	printf("Thread 1 terminated..\n");
	pthread_join(t2, NULL);
	printf("Thread 2 terminated..\n");

	traverse_list(&backup_id_list.head);
	printf("sizeof(backup_id): %d\n", sizeof(struct backup_id));
	printf("sizeof(pid): %d\n", sizeof(pid_t));

	return 0;
}
*/
