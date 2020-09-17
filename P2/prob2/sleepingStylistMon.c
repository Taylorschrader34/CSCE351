// ==== sleepingStylistMon.c ==== //
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "monitor.h"

#define CHAIRS 7
#define DELAY 1000000  //adjust this value
#define TOTALCUSTOMERS 120

pthread_t stylistThread;
pthread_t customerThread[TOTALCUSTOMERS];

void *stylistAction();
void *customerAction();

void main(){
	//Initialized the monitor for this system
	mon_init();
	
	// Creates stylist Thread for the Semaphore
	pthread_create(&stylistThread,NULL,(void *)stylistAction,NULL);
	
	// Creates customer 120 Threads for the Semaphore
	int i;
	for(i=0;i<TOTALCUSTOMERS;i++){
		pthread_create(&customerThread[i],NULL,(void *)customerAction,NULL);
	}
	
	//Joins all the Customer Threads together
	int j;
	for(j=0;j<TOTALCUSTOMERS;j++){
		pthread_join(customerThread[j],NULL);
	}
	
	//Joins the last of the threads to the stylist so that we end up serving all of the customers
	pthread_join(stylistThread,NULL);
}

void *stylistAction(){
	int j;
	while(1){
		mon_debugPrint();
		mon_checkCustomer();
		for(j=0;j<DELAY;j++); //cut hair
	}
}

void *customerAction(){
	int j;
	while(1){
		mon_debugPrint();
		if(mon_checkStylist())
			break;
		for(j=0;j<DELAY;j++); //go shopping
	}
}