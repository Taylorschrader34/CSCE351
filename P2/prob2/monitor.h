#include <semaphore.h>
#define monitor_h

typedef struct{
	int blockedThreads;
	sem_t waitThread;
}cond;

extern void mon_init();
extern void mon_signal(cond *);
extern void mon_wait(cond *);
extern void mon_checkCustomer();
extern void mon_debugPrint();
extern int mon_checkStylist();