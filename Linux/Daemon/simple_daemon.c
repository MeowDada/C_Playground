#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
    pid_t pid, sid;
    
    /* Fork off the parenet process */
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "failed to fork a child process\n");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    /* Change the file mode */
    umask(0);

    /* Open any logs here */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        fprintf(stderr, "daemon: faild to setsid()...\n");
        exit(EXIT_FAILURE);
    }

    /* Changing the working directory to the safe place */
    if ((chdir("/")) < 0) {
        fprintf(stderr, "daemon: failed to chdir to /\n");
        exit(EXIT_FAILURE);
    }

    /* Closing standard file descriptor */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* Deamon specific initialization goes here */

    /* The big loop */
    while (1) {
        /* Do some task here */
        sleep(30);
    }

    return 0;
}