#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char *string;

    printf("Please input a string: ");
    gets(string);
    printf("\nYour string is: %s\n", string);

    return 0;
}