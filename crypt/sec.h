#ifndef SEC_H	//sec = security
#define SEC_H

#define SALT 		"$1$S@n38"
#define MAX_KEY		15	//Excluding '\0'
#define MIN_KEY		8	
#define HASH_LEN	22	//MD5 is of 22 bytes (Excluding '\0')


char* get_key();
void  extract_hash(char* dest, char* src, int s, int n);
char* generate_hash(char* key);


#endif
