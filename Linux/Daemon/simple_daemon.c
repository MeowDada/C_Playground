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
    openlog("simple daemon", LOG_PID, LOG_DAEMON);

    /* Catch, ignore and handle signals */

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

    /* Redirect stdin, stdout, stderr to /dev/null */
    open("/dev/null", STDIN_FILENO);
    open("/dev/null", STDOUT_FILENO);
    open("/dev/null", STDERR_FILENO);

    /* Deamon specific initialization goes here */

    /* The big loop */
    while (1) {
        /* Do some task here */
        syslog(LOG_NOTICE, "simple daemon started.");
        sleep(30);
        break;
    }

    syslog(LOG_NOTICE, "simple daemon terminated.");
    closelog();

    return 0;
}