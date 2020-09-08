#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void* fn(void* arg)
{
	int i = 5;
	printf("Val. of i: %d\n", i);
	i++;
	printf("Val. of i: %d\n", i);
	return NULL;
}

int main()
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, fn, NULL);
	pthread_create(&t2, NULL, fn, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}
