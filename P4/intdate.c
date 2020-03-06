////////////////////////////////////////////////////////////////////////////////
// Main File:        intdate.c
// This File:        intdate.c
// Other Files:      sendsig.c, division.c
// Semester:         CS 354 Spring 2018
//
// Author:           Varun Sreenivasan
// Email:            vsreenivasan@wisc.edu
// CS Login:         sreenivasan
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          No persons.
//
// Online sources:   No online sources.
////////////////////////////////////////////////////////////////////////////////


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<time.h>
#include<string.h>

// global variable to keep track of the number of time Control-c is typed
static int sigUSR1count = 0;

/* Each time alarm() function is called, this method is called
* arg - signale
*/
void handler(int sig) {
// object of type time_t
time_t curtime;
// passing the address of time_t object, which contains the calender time 
time(&curtime);
// prints the time and date in the specified format
printf("PID: %d | Current Time: %s", getpid(), ctime(&curtime));
alarm(3);
}

/*Each time Control-c is typed, this method is called:x
* arg - signale
*/
void hndlrTime(int sig) { 
// Terminates the program
printf("\nSIGINT received.\n");
printf("SIGUSR1 was received %d times. Exiting now.\n", sigUSR1count);
exit(0); 
} 

/*
* kill command is called by user
*/
void usr1(int sig){
sigUSR1count++;
printf("SIGUSR1 caught!\n");
}

/*
* main method
*/
int main(){
// declare an struct object of type sigaction
struct sigaction sig;
// initializes all the structure members to 0
memset(&sig, 0, sizeof(sig));
// function pointer, initializing the sig handler to point to the function 
sig.sa_handler = handler;
// creates a SIGALRM handler
if (sigaction(SIGALRM, &sig, NULL) != 0) {
    perror("ERROR SETTING SIGALRM"); 
    exit(1);    
}
printf("Pid and time will be printed every 3 seconds\n");
printf("Enter ^C to end the program:\n");
// each time alarm() is executed the SIGALRM handler is called
alarm(3);

struct sigaction signal;
memset(&signal, 0, sizeof(signal));
signal.sa_handler = hndlrTime;

// creates SIGINT handler
if (sigaction(SIGINT, &signal, NULL) != 0) {
perror("ERROR SETTING SIGINT");
exit(1);
}

struct sigaction USR1signal;
memset(&USR1signal, 0, sizeof(USR1signal));
USR1signal.sa_handler = usr1; 
if (sigaction(SIGUSR1, &USR1signal, NULL) != 0){
  perror("ERROR SETTING SIGUSR1");
  exit(1);
}
while(1){}
}
