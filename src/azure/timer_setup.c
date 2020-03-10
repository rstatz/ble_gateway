#include "timer_setup.h"

/*
static void new_handler(int sig, siginfo_t *si, void *uc) {
	timer_t *tidp;
	tidp = si->si_value.sival_ptr;
	if(*tidp == timer_0)
		printf("Timer 0\n");
	else if(*tidp == timer_1)
		printf("Timer 1\n");
}*/

int make_timer(timer_t *timerid, int sec, void handler(int sig, siginfo_t *si, void *uc)) {
    struct sigevent sev;
    struct itimerspec its;
    struct sigaction sa;
    long long nano;
    printf("Establishing handler for timer signal %d\n", SIG);
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1) {
        perror(NULL);
        return 1;
    }

    /* create the timer */
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIG;
    sev.sigev_value.sival_ptr = timerid;
    if (timer_create(CLOCK_REALTIME, &sev, timerid) == -1) {
        perror(NULL);
        return 1;
    }

    printf("timer ID is 0x%lx\n", (long) *timerid);

    /* start the timer */
    its.it_value.tv_sec = sec;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    if(timer_settime(*timerid, 0, &its, NULL) == -1) {
        perror("settime");
        return 1;
    }
    return 0;
}