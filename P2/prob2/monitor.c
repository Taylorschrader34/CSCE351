// ==== monitor.c ==== //
#include<stdio.h>
#include<semaphore.h>

#define CHAIRS 7

//Creates struction for conditional

typedef struct{
	int blockedThreads;
	sem_t waitThread;
}cond;

cond StylistCond, customerCond;

//Integers that will be used for following customer que and completion 

int customerIdentifier = 0;
int stylistIdentifier = 0;
int servedCustomers = 0;

int timesSalonEmpty = 0;
int timesSalonFull = 0;
int chairsUsedArray[CHAIRS];

sem_t mutex;

//Initializes the Monitor
void mon_init(){
	sem_init(&mutex,0,1);
	sem_init(&StylistCond.waitThread,0,0);
	sem_init(&customerCond.waitThread,0,0);
	customerCond.blockedThreads=0;
	StylistCond.blockedThreads=0;
}

//Signals the monitor that a change has occured
void mon_signal(cond *cv){
	if((int) (*cv).blockedThreads == 0){
		sem_post(&mutex);
	}else{
		(*cv).blockedThreads = (*cv).blockedThreads-1;
		sem_post(&(*cv).waitThread);
	}
}

//Suspends the monitor for a time so we check back when task completed.
void mon_wait(cond *cv){
	(*cv).blockedThreads = (*cv).blockedThreads+1;
	sem_post(&mutex);
	sem_wait(&(*cv).waitThread);
}

//Monitor checks if the the customer is able to start getting their hair cut
void mon_checkCustomer(){
	sem_wait(&mutex);
	stylistIdentifier++;
	mon_signal(&StylistCond); //Stylist is ready to cut hair
	if(customerIdentifier == 0)
		mon_wait(&customerCond);
	customerIdentifier--;
	servedCustomers++;
	sem_post(&mutex);
	//return(NULL);
}

//Monitor checks if the Stylist is avialable for a haircut
int mon_checkStylist(){
	sem_wait(&mutex);
	int status = 0;
	if(customerIdentifier < CHAIRS){
		customerIdentifier++;
		mon_signal(&customerCond);
		if(stylistIdentifier == 0)
			mon_wait(&StylistCond);
		stylistIdentifier--;
		status = 1;
	}
	sem_post(&mutex);
	return status;
}

//Print function that prints out all of our useful information that tells us if the monitor is working correctly
//and information on how often the shop is busy or empty as well as what seats are taken up.
void mon_debugPrint(){
	sem_wait(&mutex);
	
	if(customerIdentifier == 0){
		timesSalonEmpty++;
	}
	if(customerIdentifier == CHAIRS){
		timesSalonFull++;
	}
	
	int k;
	for(k=0;k<CHAIRS;k++){
		chairsUsedArray[k] = 0;
	}
	for(k=0;k<customerIdentifier;k++){
		chairsUsedArray[k] = 1;
	}
	
	printf("Customer queue: %d|%d|%d|%d|%d|%d|%d => Total = %d \n", 
            chairsUsedArray[0], chairsUsedArray[1], chairsUsedArray[2], chairsUsedArray[3],
            chairsUsedArray[4], chairsUsedArray[5], chairsUsedArray[6], customerIdentifier);
    printf("Total haircuts given = %d \n", servedCustomers);
    printf("How many times the Salon has been full = %d times\n", timesSalonFull);
    printf("How many times the Salon has been empty = %d times\n", timesSalonEmpty);
	sem_post(&mutex);
}