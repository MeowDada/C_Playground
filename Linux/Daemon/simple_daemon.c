#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

static int keep_alive = 1;

static void signal_handler(int sig, siginfo_t *info, void *ptr)
{
    syslog(LOG_NOTICE, "Sending PID:%ld, UID:%ld", (long)info->si_pid, (long)info->si_uid);

    switch(sig)
    {
        case SIGINT:
            syslog(LOG_INFO, "Receiving SIGINT, start exiting the daemon...");
        case SIGSEGV:
        case SIGTERM:
        default:
            keep_alive = 0;
            break;
    }
}

static void catch_signal()
{
    struct sigaction sa;

    /* To exclude all of the defined signals */
    sigemptyset(&sa.sa_mask);

    /*  both sa.sa_sigaction (int, siginfo_t *, void *) and sa.sa_handler (int) 
        could be the signal handler function pointer, but DO NOT set both of them!
        we'll use sa.sa_sigaction here because it can do more than sa.sa_handler */
    sa.sa_sigaction = signal_handler;

    /* set sa_flag = SA_SIGINFO will use sa.sa_sigaction as handler for sure, which
       means that sa.sa_handler will NOT be used */
    sa.sa_flags = SA_SIGINFO;

    /* sa_mask specifies a mask of signals which should be blocked during execution
       of the signal handler*/
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGSEGV);
    sigaddset(&sa.sa_mask, SIGTERM);

    /* sigaction means to register and handle the signal */
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

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
    catch_signal();

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
    syslog(LOG_NOTICE, "simple daemon started.");

    /* The big loop */
    while (keep_alive) {
        /* Do some task here */
        syslog(LOG_NOTICE, "simple daemon is running.");
        sleep(30);
        break;
    }

    syslog(LOG_NOTICE, "simple daemon terminated.");
    closelog();

    return 0;
}