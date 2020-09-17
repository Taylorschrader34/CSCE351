// ==== sleepingStylistSem.c ==== //
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define CHAIRS 7
#define DELAY 1000000  //adjust this value
#define TOTALCUSTOMERS 120
sem_t mutex;
sem_t stylist;
sem_t customers;

//integers that will be used for following customer que and completion 
int servedCustomers = 1;
int currentCustomer = 1;
int customersWaiting = 0;

void *stylistAction();
void *customerAction();

pthread_t stylistThread;
pthread_t customerThread[TOTALCUSTOMERS];


void main(void){

	// Initializes all the Semaphores
	sem_init(&mutex,0,1);
	sem_init(&stylist,0,0);
	sem_init(&customers,0,0);

	// Creates stylist Thread for the Semaphore
	pthread_create(&stylistThread,NULL,(void*)stylistAction,NULL);

	// Creates customer 120 Threads for the Semaphore

	int i = 0;
	for(i; i<TOTALCUSTOMERS; i++){
		pthread_create(&customerThread[i],NULL,(void*)customerAction,NULL);
	}
	
	//Joins all the Customer Threads together
	int j =0;
	for(j;j<TOTALCUSTOMERS;j++){
		pthread_join(customerThread[j],NULL);
	}	
	
	//The way that we print out that all the customers have been served
	printf("The Stylist has now served all %d Customers\n", servedCustomers);
	
	
}

void *stylistAction(void){
	int j;
	while(1){
		sem_wait(&customers);
		sem_wait(&mutex);
		customersWaiting = customersWaiting - 1;;
		sem_post(&stylist);
		sem_post(&mutex);
		printf("\nThe Stylist is currently working on Customer %d\n", servedCustomers);
		for(j=0;j<DELAY;j++); //cut hair
		servedCustomers += 1;
	}
}

void *customerAction(void){
	int j;
	while(1){
		sem_wait(&mutex);
		printf("Customer %d has entered\n", currentCustomer);
		if(customersWaiting < CHAIRS){
			customersWaiting = customersWaiting + 1;
			printf("Customer %d is now waiting\n", currentCustomer);
			printf("Total Customers Waiting %d\n",customersWaiting);
			currentCustomer++;
			sem_post(&customers);
			sem_post(&mutex);
			sem_wait(&stylist);
			break;
		}
		else{
			sem_post(&mutex);
			//printf("Customers Waiting %d\n",customersWaiting);
			printf("There are no seats so Customer %d went shopping\n", currentCustomer);
			for(j=0;j<DELAY;j++); //go shopping
		}
	}
}