#define _XOPEN_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<crypt.h>
#include"sec.h"

//Get Password/Authentication Key from User 
//args:
//	none
//returns:
//	string containing the key
char* get_key()
{
	char* key = NULL;
	int i = 0;
	char c = '\0';

	key = calloc(MAX_KEY, sizeof(char));
	if(key == NULL)
	{
		printf("Failed to allocate memory...\n");
		exit(1);
	}

	printf("Enter Authentication Key (Min: %d and Max %d characters): ", MIN_KEY-1, MAX_KEY-1);
	while(c = getchar())
	{
		printf("i: %d\n", i);
		//If entered string's length is below the threshold, restart key input
		if((i < MIN_KEY-1 && c == '\n') || (i < MIN_KEY-1 && c == EOF)) 
		{
			printf("Error: Key must be atleast %d characters long..\n", MIN_KEY);
			printf("Re-enter Key: ");
			i = 0;	//restart again
			continue;
		}
		else if((i == MIN_KEY - 1 && c == '\n') || (i == MIN_KEY - 1 && c == EOF))
		{
			break;
		}
		//Eg:	Let, MAX_KEY = 6
		//	In this case:
		//		Input: abcde\n  is valid 
		//		      \____/\_/
		//			|    |
		//			Key  |
		//			    End of Input
		else if((i == MAX_KEY-1 && c == '\n') || (i == MAX_KEY-1 && c == EOF))
		{
			break;
		}
		//Else If Key length exceeds Max Limit
		//Or USer has entered MAX_KEY 'th char but it is not '\n' or EOF
		else if(i >= MAX_KEY-1)
		{
			//FLush Extra bytes entered by user during previous Attempt 
			while((c=getchar()) != '\n')
			{
			//	printf("Flushing: %c %d\n", c, c);	
			}
			//printf("Flushed all data..\n");
			printf("Error: Key must be atmost %d characters long..\n", MAX_KEY);
			printf("Re-enter Key: ");
			i = 0;	//restart again
			continue;
		}
		//Key length is within Permissible limits and User wants to end it
		else if(c == '\n' || c == EOF)
		{
			printf("Breaking..\n");
			break;
		}
		//If everything is legal, assign scanned character in key array
		else
		{
			key[i++] = c;
		}
	}
	//terminate Key
	key[i] = '\0';

	return key;
}

//args:
//	dest:	character array where hash will be stored after extraction
//	src:	character array from which hash is to be extracted
//	s:	starting byte from which extraction should start (0 indexed)
//	n:	number of bytes to be extracted (including s'th byte)
//
//returns:
//	Nothing
//Notes:
//	It is assumed that, input will be correct.
//	No error checking is performed in this function
//
void extract_hash(char* dest, char* src, int s, int n)
{
	int i = 0;
	i = s;
	printf("Extracting Hash..\n");
	while(i < s + n)
	{
		dest[i-s] = src[i];
		printf("dest[%d-%d]: %c src[i]: %c\n", i, s, dest[i-s], src[i]);
		i++;
	}
	printf("Extraction..\n");
}


//generate hash corresponding to the passed Key
//args:
//	key:	character string entered by user as key/password	
//returns:
//	hash:	character string containing the generated hash
char* generate_hash(char* key)
{
	char* crypt_out;
	char* hash;
	hash = calloc(HASH_LEN+1, sizeof(char));
	if(hash == NULL)
	{
		printf("Failed to allocate memory...\n");
		exit(1);
	}

	//generate string containing hash
	crypt_out = crypt(key, SALT);

	//printf("Key: %s\n", key);
	//printf("Size of salt: %d\n", sizeof(SALT));
	//printf("Encrypted Password: %s\n", enc_passwd);

	//extract hash from obtained string
	extract_hash(hash, crypt_out, sizeof(SALT), HASH_LEN+1); 

	//printf("Hash: %s\n", hash);

	return hash;
}
