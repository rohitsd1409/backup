#ifndef STDOPS_H	
#define STDOPS_H	//STDOPS = standard operations

#define BACKUP_DIR 	"/tmp/backup"	//main directory that will house all backup related files
#define DIRMODE_0700	0700			//Common mode that will be used to create some dir's
#define DIRMODE_0755	0755			//Common mode that will be used to create some dir's

#define preturn(str)	perror(#str);return -1;	//perror + return

int make_dir(char* name, mode_t mode, int remove_existing_ifdiff);	//create new dir. based on supplied args.
char* str_diff(char* str1, char* str2);	//compare 2 strings and return a string of characters from the first char onwards where 2 strings differ

#endif
