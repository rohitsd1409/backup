#ifndef FILESTATS_H
#define FILESTATS_H

#include<sys/stat.h>

//Extract timestamps from supplied stat buffer and apply them on a file
int preserve_timestamps(char* abs_path_file, time_t* atime, time_t* mtime );
//Extract Ownership Details from supplied stat buffer and apply them on a file
int preserve_ownership(char* abs_path_file, uid_t* uid, gid_t* gid);

#endif
