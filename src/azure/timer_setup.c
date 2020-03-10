#include "timer_setup.h"

int make_timer(timer_t *timerid, int sec, void handler(int sig, siginfo_t *si, void *uc)) {
    struct sigaction sa;
    printf("Establishing handler for timer signal %d\n", SIG);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1) {
        perror(NULL);
        return 1;
    }
    return 0;
}