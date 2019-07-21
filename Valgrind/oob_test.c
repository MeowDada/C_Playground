#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int len = 5;
    int i;
    int *ptr = (int*)malloc(len*sizeof(int));
    int *p = ptr;
    
    for (i = 0; i < len; i++)
        p++;
    *p = 5;
    
    printf("%d\n", *p);

    return 0;
}