#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define INTERVAL 900000

struct itimerval clocktimer;

void producer(void);
void signal_processor( int signal);
int main( void )
{
	   /* initialize timer to send signal every 300 ms */
	clocktimer.it_value.tv_sec = 0;
	clocktimer.it_value.tv_usec = INTERVAL;
	clocktimer.it_interval.tv_sec = 0;
	clocktimer.it_interval.tv_usec = INTERVAL;
	setitimer (ITIMER_REAL, &clocktimer, 0);
	sigset (SIGALRM, signal_processor);
	sigset (SIGINT, signal_processor);
	producer();
}

void signal_processor( int signal )
{
	if(signal == SIGALRM)
	   printf ("get a signal (%d)\n", signal);
	else
	   printf ("get an interrupt (%d)\n", getpid());
}
void producer()
{
	int i;
	while(1){
		for (i = 0; i < 100000000; i++);
		printf ("Producer\n");
	}		
}
