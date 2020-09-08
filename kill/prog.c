#include<stdio.h>
#include<sys/types.h>
#include<signal.h>

int main(int argc, char* argv[])
{
	int pid;
	pid = atoi(argv[1]);

	printf("%d\n", kill(pid, 0));

}
