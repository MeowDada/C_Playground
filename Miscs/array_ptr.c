#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv)
{
	int a[5] = {1,2,3,4,5};
	int i;
	for(i = 0; i < 5; i++)
	{
		printf("%d ", i[a]);
	}
	printf("\n");

	int b[10];
	printf("int b[10];\n");
	printf("mem address of b:      %p\n", b);
	printf("mem address of (b+1):  %p\n", b+1);
	printf("mem address of &b:     %p\n", &b);
	printf("mem address of &(b+1): %p\n", &b+1);

	printf("\n");
	
	int *c = malloc(sizeof(int)*10);
	printf("int *c = malloc(sizeof(int)*10)\n");
	printf("mem address of c:      %p\n", c);
	printf("mem address of (c+1):  %p\n", c+1);
	printf("mem address of &c:     %p\n", &c);
	printf("mem address of &c+1:   %p\n", &c+1);

	return 0;
}
