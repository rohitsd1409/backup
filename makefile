

sanbackup: sanbackup.c ipc.o sec.o -lcrypt
	gcc -Wall -o $@ $^

sanbackup_remote: sanbackup_remote.c ipc.o sec.o -lcrypt
	gcc -Wall -o $@ $^

sec.o:	sec.c sec.h
	gcc -c $<

backup_daemon: backup_daemon.c filestats.o ipc.o lock.o stdops.o backup_id.o backup.h 
	gcc -Wall -o $@ $^ -lpthread


#backup.o: backup.c backup.h
#	gcc -c $<
backup_id.o: backup_id.c backup_id.h
	gcc -Wall -c $< -lpthread

stdops.o: stdops.c stdops.h
	gcc -c $<

ipc.o: ipc.c ipc.h
	gcc -c $<

filestats.o: filestats.c filestats.h
	gcc -c $<

lock.o: lock.c lock.h
	gcc -c $< 
clean:
	rm -f sanbackup backup_daemon *.o
