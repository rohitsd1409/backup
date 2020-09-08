#ifndef LOCK_H
#define LOCK_H

#define  DUMMY_FILENAME "/tmp/backup/dummy"             //dummy file to make sure that only one instance of daemon runs


int single_instance_check();	//make sure that only one instance of the daemon is running

#endif
