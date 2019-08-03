#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include "common.h"

/*  Singals:
 *      SIGHUP      :Hangup                             (POSIX)
 *      SIGINT      :Interrupt                          (ANSI)
 *      SIGQUIT     :Quit                               (POSIX)
 *      SIGILL      :Illegal instruction                (ANSI)
 *      SIGTRAP     :Trace trap                         (POSIX)
 *      SIGABRT     :Abort                              (ANSI)
 *      SIGIOT      :IOT trap                           (4.2 BSD)
 *      SIGBUS      :Bus error                          (4.2 BSD)
 *      SIGFPE      :Floating point exception           (ANSI)
 *      SIGKILL     :Kill, unblockable                  (POSIX)
 *      SIGUSR1     :User-define signal 1               (POSIX)
 *      SIGSEGV     :Segementation violation            (ANSI)
 *      SIGUSR2     :User-define signal 2               (POSIX)
 *      SIGPIPE     :Broken pipe                        (POSIX)
 *      SIGALRM     :Alarm clock                        (POSIX)
 *      SIGTERM     :Termination                        (ANSI)
 *      SIGSTKFLT   :Stack fault
 *      SIGCLD      :Same as SIGCHLD                    (System V)
 *      SIGCHILD    :Child status has changed           (POSIX)
 *      SIGCONT     :Continue                           (POSIX)
 *      SIGSTOP     :Stop, unblockable                  (POSIX)
 *      SIGTSTP     :Keyboard stop                      (POSIX)
 *      SIGTTIN     :Background read from tty           (POSIX)
 *      SIGTTOU     :Background write to tty            (POSIX)
 *      SIGURG      :Urgent condition on socket         (4.2 BSD)
 *      SIGXCPU     :CPU limit exceeded                 (4.2 BSD)
 *      SIGXFSZ     :File size limit exceeded           (4.2 BSD)
 *      SIGVTALRM   :Virtual alarm clock                (4.2 BSD)
 *      SIGPROF     :Profiling alarm clock              (4.2 BSD)
 *      SIGWINCH    :Window size change                 (4.3 BSD, Sun)
 *      SIGPOLL     :Pollable event occurred            (System V)
 *      SIGIO       :I/O now possible                   (4.2 BSD)
 *      SIGPWR      :Power failure restart              (System V)
 *      SIGSYS      :Bad system call                    
 *      SIGUNUSED   :
 */

#define SET_TIMER(timer, value) \
do {    \
    if (setitimer(timer, value, NULL) != 0) {   \
        LOGGING_ERROR("setitimer failed");  \
    }   \
} while(0); \

#define GET_TIMER(timer, value) \
do {    \
    if (getitimer(timer, value) != 0) { \
        LOGGING_ERROR("getitimer failed");  \
        goto end;   \
    }   \
} while(0); \

#define LOG_TIMER(log_info, value) \
do {    \
    LOGGING_INFO(log_info,   \
        value.it_interval.tv_sec, value.it_interval.tv_usec,    \
        value.it_value.tv_sec, value.it_value.tv_usec); \
} while(0); \

int loop_is_done = 0;
int receive_sigalrm = 0;
int receive_sigvtalrm = 0;
int receive_sigprof = 0;

void setup_signal_handler(void);

void check_received_signal(void)
{
    if (receive_sigalrm && receive_sigvtalrm && receive_sigprof) {
        loop_is_done = 1;
    }
}

void handle_sigalrm(int signal)
{
    LOGGING_INFO("Received signal SIGALRM");
    LOGGING_INFO("Stop timer ITIMER_REAL");
    struct itimerval stop;
    stop.it_interval.tv_sec  = 0;
    stop.it_interval.tv_usec = 0;
    stop.it_value.tv_sec     = 0;
    stop.it_value.tv_usec    = 0;
    setitimer(ITIMER_REAL, &stop, NULL);
    receive_sigalrm = 1;
    check_received_signal();
}

void handle_sigvtalrm(int signal)
{
    LOGGING_INFO("Received signal SIGVTALRM");
    LOGGING_INFO("Stop timer ITIMER_VIRTUAL");
    struct itimerval stop;
    stop.it_interval.tv_sec  = 0;
    stop.it_interval.tv_usec = 0;
    stop.it_value.tv_sec     = 0;
    stop.it_value.tv_usec    = 0;
    setitimer(ITIMER_VIRTUAL, &stop, NULL);
    receive_sigvtalrm = 1;
    check_received_signal();
}

void handle_sigprof(int signal)
{
    LOGGING_INFO("Received signal SIGPROF");
    LOGGING_INFO("Stop timer ITIMER_PROF");
    struct itimerval stop;
    stop.it_interval.tv_sec  = 0;
    stop.it_interval.tv_usec = 0;
    stop.it_value.tv_sec     = 0;
    stop.it_value.tv_usec    = 0;
    setitimer(ITIMER_PROF, &stop, NULL);
    receive_sigprof = 1;
    check_received_signal();
}

void setup_signal_handler(void)
{
    signal(SIGALRM, handle_sigalrm);
    signal(SIGVTALRM, handle_sigvtalrm);
    signal(SIGPROF, handle_sigprof);
}

void setup_timer(struct itimerval *real,
    struct itimerval *virtual, struct itimerval *prof)
{
    real->it_interval.tv_sec  = 5;
    real->it_interval.tv_usec = 0;
    real->it_value.tv_sec     = 5;
    real->it_value.tv_usec    = 0;
    SET_TIMER(ITIMER_REAL, real);

    virtual->it_interval.tv_sec  = 2;
    virtual->it_interval.tv_usec = 0;
    virtual->it_value.tv_sec     = 2;
    virtual->it_value.tv_usec    = 0;
    setitimer(ITIMER_VIRTUAL, virtual, NULL);

    prof->it_interval.tv_sec  = 1;
    prof->it_interval.tv_usec = 0;
    prof->it_value.tv_sec     = 1;
    prof->it_value.tv_usec    = 0;
    setitimer(ITIMER_PROF, prof, NULL);
}

int main(int argc, char **argv)
{
    setup_logger(stdout, LOG_LEVEL_INFO);

    struct itimerval real, virtual, prof;
    struct itimerval value;


    setup_signal_handler();
    setup_timer(&real, &virtual, &prof);

    while (!loop_is_done) {
        struct itimerval value_real, value_virtual, value_prof;

        GET_TIMER(ITIMER_REAL, &value_real);
        LOG_TIMER("\bITIMER_REAL    :interval: %8ds %dms, remain: %8ds %dms\r", value_real);

        GET_TIMER(ITIMER_VIRTUAL, &value_virtual);
        LOG_TIMER("\bITIMER_VIRTUAL :interval: %8ds %dms, remain: %8ds %dms\r", value_virtual);
        
        GET_TIMER(ITIMER_PROF, &value_prof);
        LOG_TIMER("\bITIMER_PROF    :interval: %8ds %dms, remain: %8ds %dms\r", value_prof);
        
        for(int i = 0; i< 100000000; i++);
        usleep(1000);
    }
end:
    LOGGING_INFO("close_logger...");
    close_logger(NULL);

    return 0;
}