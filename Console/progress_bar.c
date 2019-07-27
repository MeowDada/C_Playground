#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define LOOP 10000

int main(int argc, char **argv)
{
    int i = 0;
    for (i = 0; i < LOOP; i++) {
        printf("\bIn progress: %3d%c", i / (LOOP/100), '%');
        printf("\r");
        fflush(stdout);
#ifdef _WIN32
        Sleep(0.001);
#else
        usleep(1000);
#endif
    }

    return 0;
}