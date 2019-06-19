#include<stdlib.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char** argv)
{
	const char * str = "qnaper";
	
	printf("sizeof(str)=%d\n", sizeof(str));
	printf("strlen(str)=%d\n", strlen(str));
	printf("strlen(str+1)=%d\n", strlen(str+1));
	
	const char * new_str = strdup(str);
	
	system("pause");
	
	return 0;
}
