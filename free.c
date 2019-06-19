#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
	int num = 8;
	int *ptr = (int*)malloc(sizeof(int));
	ptr = &num;
	
	printf("num = %d\n", num);
	printf("*ptr = %d\n", *ptr);
	printf("ptr = %d\n", ptr);
	
	printf("Then do free(ptr)\n");
	free(ptr);
	
	printf("num = %d\n", num);
	printf("CANNOT ACCESS *ptr\n");
	printf("ptr = %d\n", ptr);
	
	return 0;
}
