#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>

#ifndef TIMER_SETUP_H
#define TIMER_SETUP_H

#define SIG SIGRTMIN

int make_timer(timer_t *timerid, int sec, void handler(int sig, siginfo_t *si, void *uc));

#endif
