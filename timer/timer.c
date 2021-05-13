#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>

void init()
{
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
}


static void timerHandler( int sig, siginfo_t *si, void *uc )
{
    timer_t *tidp;
    tidp = si->si_value.sival_ptr;
	puts("handler");	
}

// implement multiple timer
static int makeTimer( char *name, timer_t *timerID, int expireSeconds, int intervalSeconds )
{
    struct sigevent         te;
    struct itimerspec       its;
    struct sigaction        sa;
    int sigNo = SIGRTMIN;

    /* Set up signal handler. */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = timerHandler;
    sigemptyset(&sa.sa_mask);

    if (sigaction(sigNo, &sa, NULL) == -1) {
		perror("sigaction");
        return(-1);
    }

    /* Set and enable alarm */
    te.sigev_notify = SIGEV_SIGNAL;
    te.sigev_signo = sigNo;
    te.sigev_value.sival_ptr = timerID;
    timer_create(CLOCK_REALTIME, &te, timerID);
    its.it_interval.tv_sec = intervalSeconds;
    its.it_interval.tv_nsec =0;

    its.it_value.tv_sec = expireSeconds;
    its.it_value.tv_nsec = 0;
    timer_settime(*timerID, 0, &its, NULL);
    return(0);
}


int main(int argc, char** argv)
{
	timer_t tid;
	makeTimer("No.1", &tid, 5, 0);	
	while(1)
	{"main loop";}
    return EXIT_SUCCESS;
}
