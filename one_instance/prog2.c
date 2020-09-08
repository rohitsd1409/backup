#include<stdio.h>

void fn()
{
	printf("fn() called..\n");
}
int main()
{
	atexit(fn);
	printf("Calling pause()..\n");
	pause();
	printf("Returned from  pause()..\n");
	return 0;
}
