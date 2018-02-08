/*************************************************
 * *Lab 4 Programming Assignment
 * *Done By Adam Slifco & Michael Ames
 * *February 8th 2018
 * ***********************************************/
#include <pthread.h> 
#include <stdio.h> 
#include <errno.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <string.h>
#include <signal.h>

//declare global functions
void* file_handle (void* arg);
void sigHandler (int);

//shared data 
int running = 1;

//Global mutex lock to prevent Mutual Exclusion
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){

//User Input
char input[100];

pthread_t worker[20];

int status;
int fileRequests = 0;

signal(SIGINT, sigHandler);

while(running){
	
	//Prompt user to enter filename's and retrieve input
	printf("Enter the filename's you wish to access: \n");
	fgets(input, 100, stdin);
	input[strlen(input) - 1] = '\0';
	
	//Handle breaking loop if user CTRL-C's	
	if(running == 0) { 
	break; 
	}

	//prevents blank input executions
	if(strlen(input) != 0){

		//Spawn Child/Worker thread
		if((status = pthread_create (&worker[fileRequests], NULL,  file_handle, (void *)input)) != 0) { 
        	fprintf (stderr, "thread create error %d: %s\n", status, strerror(status)); 
        	exit (1); 
    		}
		//Increment number of threads/requests 
		else {
		fileRequests++;
		} 
	} 
	else {
	printf("No File was Entered...\n");
	}
}
	//Upon CTRL-C terminations, wait for working threads to finish
	int i;
	for(i = 0; i < fileRequests; i++){
		 pthread_join(worker[i],NULL);
	}
	//Display number of files accessed
	printf("Number of Files Accessed: %i\n", fileRequests);
	return 0;
}

void* file_handle (void* arg){
	
	pthread_mutex_lock(&mutex);
	//Generate random num
	int sleepProb = rand() % 100;
	int randSleep;

	//Display filename, casts "void *" type arg to "char *" type
	printf("Worker processing file: %s\n",(char *) arg);

	//Randomly sleep for 1 or 7 - 10 seconds
	if(sleepProb <= 20){
		randSleep = (rand() % 4) + 7;
		sleep(randSleep);			
	}
	else
	{
		sleep(1);
	}
	//Update user that file accessing is finished
	printf("Worker finished accessing file: %s\n",(char *) arg);

	pthread_mutex_unlock(&mutex);
	return NULL;
}

void sigHandler (int sigNum){
    if(sigNum == SIGINT){
        running = 0;
    }                 
}

