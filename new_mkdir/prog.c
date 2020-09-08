#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>


//#define BACKUP_DIR "dir"
//#define mode 0700
#define perr(str) perror(#str);return -1;

//Compare two mode_t values and check whether they are equal or not
//Note: As of now, we are interested in file permissions only
//
//returns:
//	1:	equal
//	0:	unequal
static int isequal_mode(mode_t mode1, mode_t mode2)
{
/*	printf("mode1: %d mode2: %d\n", mode1, mode2);
	printf("%d\n", (mode1 & S_IRUSR) == (mode2 & S_IRUSR)); 
	printf("%d\n", (mode1 & S_IWUSR) == (mode2 & S_IWUSR));
	printf("%d\n", (mode1 & S_IXUSR) == (mode2 & S_IXUSR));
	printf("%d\n", (mode1 & S_IRGRP) == (mode2 & S_IRGRP));
	printf("%d\n", (mode1 & S_IWGRP) == (mode2 & S_IWGRP));
	printf("%d\n", (mode1 & S_IXGRP) == (mode2 & S_IXGRP));
	printf("%d\n", (mode1 & S_IROTH) == (mode2 & S_IROTH));
	printf("%d\n", (mode1 & S_IWOTH) == (mode2 & S_IWOTH));
	printf("%d\n", (mode1 & S_IXOTH) == (mode2 & S_IXOTH));
*/
	return (((mode1 & S_IRUSR) == (mode2 & S_IRUSR)) &&
		((mode1 & S_IWUSR) == (mode2 & S_IWUSR)) &&
		((mode1 & S_IXUSR) == (mode2 & S_IXUSR)) &&
		((mode1 & S_IRGRP) == (mode2 & S_IRGRP)) &&
		((mode1 & S_IWGRP) == (mode2 & S_IWGRP)) &&
		((mode1 & S_IXGRP) == (mode2 & S_IXGRP)) &&
		((mode1 & S_IROTH) == (mode2 & S_IROTH)) &&
		((mode1 & S_IWOTH) == (mode2 & S_IWOTH)) &&
		((mode1 & S_IXOTH) == (mode2 & S_IXOTH))
		);
}


//make a directory
//args:
//	name: character string representing name of the dir to be created
//	mode: dir mode/permissions
//	remove_existing_ifdiff: flag to indicate
//					if file with passed "name" already exists remove the existing file if:
//						* it is not a dir. 
//						* it is a dir. but mode of already existing dir doesnot match with the passed mode
//		value 1: perform removal if above conditions are satisfied	
//		value 0: perform no operation, i.e. exit
//
//returns:
//	-1: indicates that requested dir. wasn't created
//	0 : success
//		
int make_dir(char* name, mode_t mode, int remove_existing_ifdiff)
{
	struct stat buf; 
	int temp_pid;
	errno = 0;

	if(-1 == mkdir(name, mode))
	{    
		//file with passed name already exists and it shouldn't be removed
		if((errno == EEXIST) && (remove_existing_ifdiff == 0))
		{
			printf("%s already exists..\n", name);
			printf("Exiting dir creation..\n", name);
			return -1;
		}
		//file with passed name already exists
		//	* remove it if if has diff. parameters than what we are looking for
		//		(it must be a dir with mode that is supplied in the args, otherwise remove it and 
		//		create dir with name and mode that we have supplied)
		if((errno == EEXIST) && (remove_existing_ifdiff == 1))
		{    
			printf("%s already exists..\n", name);
			if(-1 == lstat(name, &buf))
			{    
				perr("lstat");
			}
			if(!S_ISDIR(buf.st_mode) || !isequal_mode(mode, buf.st_mode))
			{
				printf("Either %s exists but it is not a directory OR modes differ..\n", name);
				printf("Removing Existing: %s\n", name);
				if((temp_pid = fork()) == -1)
				{    
					perr("fork");
				}    
				else if(temp_pid == 0)
				{    
					printf("Inside Child process..\n");
					if(-1 == execlp("rm", "rm", "-r", name, NULL))
					{    
						perr("execlp");
					}    
				}    
				wait(&temp_pid);
				printf("%s unlinked..\n", name);
				//Create Requested dir
				if(-1 == mkdir(name, mode))
				{
					perr("mkdir");
				}

			}
			else
			{
				printf("Dir with requested name: %s and mode already exists..\n", name);
				return 0;
			}
		}
		else
		{
			perr("mkdir");
		}
	}
	printf("Dir with requested name: %s and mode created successfully..\n", name);

	return 0;
}

int main()
{

	printf("Returned Value: %d\n", make_dir("????", 0755, 1));
	return 0;
}
