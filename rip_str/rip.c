#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//compare two strings until diff. character is found
//and return ripped string from diff char. onwards
//args:
//	str1:	larger string
//	str2:	smaller string
//returns:
//	ripped char string of which str1 and str2 differ
//Eg:
//	str_diff_tail("Hello/World", "Hello");
//	returns:
//		/World
char* str_diff(char* str1, char* str2)
{
	int i=0;
	int j=0;
	int n=0;
	int diff_len=0;

	n = strlen(str1);
	while(1)
	{
		if(str1[i] == str2[i])
		{
			i++;
		}
		else
		{
			--i;
			break;
		}
	}
	diff_len = n-i;
	printf("%d %d %d\n", diff_len, n, i);
	char* diff_str = (char*)malloc(diff_len);
	for(j=0; j < diff_len; j++)
	{
		diff_str[j]=str1[++i];
		printf("%c\n", diff_str[j]);
	}

	return diff_str;
}

/*
int main()
{
	char* str1 = "/bin/ls/2015";
	char* str2 = "/bin/ls/";
	printf("ripped string: %s\n", str_diff_tail(str1, str2));
	return 0;
}
*/
