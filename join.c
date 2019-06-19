#include <stdio.h>
#include <pthread.h>

#define N_THREADS 10

void *thread_func(void*);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;

int main(int argc, char** argv)
{
	pthread_t thread_id[N_THREADS];
	int i, j;
	
	for(i = 0 ; i < N_THREADS ; i++)
		pthread_create(&thread_id[i], NULL, (void*)&thread_func, NULL);
	
	for(j = 0 ; j < N_THREADS ; j++)
		pthread_join(thread_id[j], NULL);
	
	printf("Final counter value: %d\n", counter);
	
	return 0;
}

void *thread_func(void* dummyPtr)
{
	(void)dummyPtr;
	
	printf("Thread number %ld\n", pthread_self());
	pthread_mutex_lock(&mutex);
	counter++;
	pthread_mutex_unlock(&mutex);
}
