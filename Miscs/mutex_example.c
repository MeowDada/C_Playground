#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *incrementCounter();
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0; 

int main(int argc, char** argv)
{
	int rc1, rc2;
	pthread_t thread1, thread2;
	
	if((rc1 = pthread_create(&thread1, NULL, (void*)&incrementCounter, NULL))){
		printf("Thread creation failed: %d\n", rc1);
	}
	
	if((rc2 = pthread_create(&thread2, NULL, (void*)&incrementCounter, NULL))){
		printf("Thread creation failed: %d\n", rc2);
	}
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	exit(0);
}

void *incrementCounter()
{
	pthread_mutex_lock(&mutex);
	counter++;
	printf("Counter value: %d\n", counter);
	pthread_mutex_unlock(&mutex);
}
