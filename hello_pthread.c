#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function(void *ptr);

int main(int argc, char** argv)
{
	pthread_t thread1, thread2;
	const char *message1 = "Thread 1";
	const char *message2 = "Thread 2";
	
	int iret1 = pthread_create(&thread1, NULL, print_message_function, (void*)message1);
	if(iret1 != 0){
		fprintf(stderr, "pthread_create thread failed!");
		return 0;
	}
	
	int iret2 = pthread_create(&thread2, NULL, print_message_function, (void*)message2);
	if(iret1 != 0){
		fprintf(stderr, "pthread_create thread failed!");
		return 0;
	}
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	
	printf("Thread 1 returns: %d\n", iret1);
    printf("Thread 2 returns: %d\n", iret2);
    exit(0);	
}

void *print_message_function(void *ptr)
{
	const char *message;
	message = (const char*)ptr;
	printf("%s\n", message);
}
