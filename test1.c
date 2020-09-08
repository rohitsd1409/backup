#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
	char* str;
	str = malloc(sizeof(char) * 50);
	printf("%d\n", strcmp(str, ""));
	free(str);
}
