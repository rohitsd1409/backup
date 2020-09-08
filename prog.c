#include"sec.h"
#include<stdio.h>
#include<stdlib.h>

int main()
{
	char* key;
	char* hash;

	key = get_key();	
	hash = generate_hash(key);	
	printf("Hash: %s\n", hash);

	free(key);
	free(hash);

	return 0;
}
