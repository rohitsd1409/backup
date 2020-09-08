#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include"ipc.h"
#include"backup.h"

//print help regarding how to use this tool
void usage()
{
        printf("Usage:\n\n");
        printf(" sanbackup [-f nmin] <src> <dest> [<Dest. IP address>]\n");
        printf(" sanbackup -f nmin\n");
        printf(" sanbackup -l [<timestamp>] \n");
        printf(" sanbackup -r <\"*\"/file/dir> <newdir> [<timestamp>]\n");
	printf("\nNote: \n\n");
	printf(" Path Limit: %d chars\n", PARAM_SIZE-1);
	printf(" Frequency Limit: %d chars\n", FREQ_LEN-1);
	printf(" Timestamp Limit: %d chars\n", TIMESTAMP_LEN-1);
	printf(" Only IPv4 supported as of now.\n");
}

//returns:
//      1, if valid frequency is supplied
//      0, otherwise
int isvalid_frequency(char* ptr)
{
        int i = 0;
        char c;

        while((c = ptr[i++]) != '\0')
        {
                //make sure that each char is actually a digit  and  length of arguement supplied is valid
		//since, each char should be a digit, this means, we have automatically handled the case of
		//frequency being non-negative
                if( (!( c >= 48 && c <= 57)) || ( i > PARAM_SIZE))
                {
                        printf("Invalid frequency!!\n");
                        return 0;
                }
        }

        return 1;
}

//returns:
//      1, if valid timestamp is supplied
//      0, otherwise
int isvalid_timestamp(char* ptr)
{
        int i = 0;
        char c;

        //make sure arguement is of valid length
        if(strlen(ptr) != TIMESTAMP_LEN)
        {
                printf("Invalid length of timestamp\n");
                return 0;
        }

        //make sure that arguement contains only digits
        for(i = 0; i < TIMESTAMP_LEN; i++)
        {
                c = ptr[i];

                //make sure that each char is actually a digit  and  length of arguement supplied is valid
                if(!( c >= 48 && c <= 57))
                {
                        printf("character found in timestamp\n");
                        return 0;
                }
        }

        printf("Valid timestamp!\n");
        return 1;
}


//Check Whether the supplied value of IP address is in correct string format
//	or not
//args:
//	ip:	character string entered by user as IP address
//returns:
//	1:	if IP string formatting is correct
//	0:	otherwise
//NOTE:
//	As of now, now IPv4 is Supported
int isvalid_ip(char* ip)
{
	struct sockaddr_in s;	
	if(inet_pton(AF_INET, ip, &s.sin_addr))
	{
		printf("inet_pton(): Valid IP String\n");
		return 1;	//Success
	}
	printf("inet_pton(): Invalid IP String\n");
	return 0;	//Failure
}
