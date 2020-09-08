#ifndef SEC_H	//sec = security
#define SEC_H

#define SALT 		"$1$S@n38"
#define MAX_KEY		16	//Including '\0'
#define MIN_KEY		6	//Including '\0'	
#define HASH_LEN	23	//MD5 is of 22 bytes + 1 (for '\0')


char* get_key();
void  extract_hash(char* dest, char* src, int s, int n);
char* generate_hash(char* key);


#endif
