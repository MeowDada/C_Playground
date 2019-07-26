#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv)
{
    size_t num_0 = 0, num_1 = 0;

    printf("Assign -1 to size_t variable num_0 and see what will happen...\n");
    num_0 = -1;
    printf("(size_t)num_0 = %lu\n", num_0);
    printf("\n");
    printf("Set num_0 = 10, num_1 = 100, and see what will happen if num_0 - num_11...\n");
    num_0 = 10;
    num_1 = 100;
    printf("num_0 - num_1 = %lu\n", num_0 - num_1);

#ifdef _WIN32
    system("pause");
#else
#endif

    return 0;
}