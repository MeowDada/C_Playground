#include<stdio.h>
#include<stdlib.h>

struct foo{
	int x;
};

void bar(int *ptr, struct foo *foo_)
{
	*ptr = 3;
	foo_->x = 5;
}

int main(int argc, char** argv)
{
	struct foo *foo_t = (struct foo*)malloc(sizeof(struct foo));
	foo_t->x = 6;
	int num = 8;
	int *ptr;
	ptr = &num;
	
	printf("num = %d\n", num);
	printf("ptr = %d\n", *ptr);
	printf("foo_t.x = %d\n", foo_t->x);
	printf("after foo(...)\n");
	bar(ptr, foo_t);
	printf("num = %d\n", num);
	printf("ptr = %d\n", *ptr);
	printf("foo_t.x = %d\n", foo_t->x);
	
	system("pause");
	
	return 0;	
}
