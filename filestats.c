#include<sys/stat.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

//
//Preserve timestamps of a file
//Args:
//	abs_path_file:	absolute path of the file whose timestamps are to be updated (preserved) with supplied values
//	atime:		val. of last access time to be set
//	mtime:		val. of last modification time to be set
//
//Returns:
//	-1 : failed to preserve timestamps
//	0  : Otherwise
int preserve_timestamps(char* abs_path_file, time_t* atime, time_t* mtime)
{
	struct timespec times[2];

	times[0].tv_sec  = *atime;
	times[0].tv_nsec = 0;
	times[1].tv_sec  = *mtime;
	times[1].tv_nsec = 0;

	printf("preserve_timestamps() Called\n");
	if(-1 == utimensat(0, abs_path_file, times, AT_SYMLINK_NOFOLLOW))
	{
		perror("utimensat");
		return -1;
	}

	return 0;
}






//
//Preserve Ownership of a file
//Args:
//	abs_path_file:	absolute path of the file whose Ownership is to be updated (preserved) with supplied values
//
//	uid:	user id of the user to be set as owner
//	gid:	group id of the group to be set as owner
//Returns:
//	-1 : failed to preserve Ownership 
//	0  : Otherwise
int preserve_ownership(char* abs_path_file, uid_t* uid, gid_t* gid)
{
	printf("preserve_ownership() Called\n");
	if(-1 == chown(abs_path_file, *uid, *gid))
	{
		perror("chown");
		return -1;
	}

	return 0;
}


