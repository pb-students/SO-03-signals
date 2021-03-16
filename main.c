#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <syslog.h>

volatile bool running = true;

sig_t addSignalHandler (int signum, void* handler) {
    sig_t res = signal(signum, handler);
    if (res == SIG_ERR) {
        syslog(LOG_ERR, "Error when adding signal handler: ");
        fprintf(stderr, "Error when adding signal handler: ");
        switch (errno) {
            case EINVAL:
                syslog(LOG_ERR, "Invalid signal number.");
                fprintf(stderr, "Invalid signal number.");
                break;
        }

        fprintf(stderr, "\n");
    }

    return res;
}

void handler(int signum) {
    syslog(LOG_NOTICE, "Signal received: %s", strsignal(signum));
    running = false;
}

int main(int argc, char* argv[]){
    addSignalHandler(SIGQUIT, handler);

    setlogmask(LOG_UPTO(LOG_NOTICE));
    openlog(argv[0], LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog(LOG_NOTICE, "Program started by User %d", getuid ());

    while (running) {
        printf("working...\n");
        sleep(1);
    }

    printf("stopped\n");
    closelog();

    return 0;
}