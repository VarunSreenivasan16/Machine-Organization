////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      intdate.c, sendsig.c
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

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

// global variable to keep count of how many divisions operations are completed
static int count = 0;

/* Each time divison by 0 occurs this method is called
* arg- signal
*/
void handler(int sig) {
  // print the following statements
  printf("Error: a division by 0 operation was attempted.\n");
  printf("Total number of operations completed successfully: %d\n", count);
  printf("The program will be terminated.\n");
  // Terminates the program
  exit(0);
}

/* Each time Control-c is typed, this method is called
* arg - signal
*/
void hndlrCopy(int sig) {
  // print the following statements
  printf("\nTotal number of operations successfully completed: %d\n", count);
  printf("The program will be terminated.\n");
  // Terminates from the prgram
  exit(0);
}

/*
* main method
*/
int main()
{
  // character Buffer of size 100 bytes
  char Buffer[100];
  // declaring struct of type sigaction
  struct sigaction sig;
  // Initializing all the data members to 0
  memset(&sig, 0, sizeof(sig));
  // function pointer to handler method, which is pointed by the sig handler 
  sig.sa_handler = handler;

  // creates SIGFPE handler
  if (sigaction(SIGFPE, &sig, NULL) != 0) {
    perror("ERROR SETTING SIGFPE");
    exit(1);
  }

  struct sigaction signal;
  memset(&signal, 0, sizeof(signal));
  signal.sa_handler = hndlrCopy;

  // creates SIGINT handler
  if (sigaction(SIGINT, &signal, NULL) != 0) {
     perror("ERROR SETTING SIGINT");
     exit(1);
  }

  while (1) {
    printf("Enter first integer: ");
    // reads the data from standard input(keyboard) and stores it in the buffer
    if (fgets(Buffer, 100, stdin) == NULL){
      perror("Unable to read input from console\n");
      exit(1); 
    }
    // implement the atoi() function to convert the data to an integer
    int temp = atoi(Buffer);

    printf("Enter second integer: ");

    if (fgets(Buffer, 100, stdin) == NULL){
       perror("Unable to read input from console\n");
       exit(1);
    }
    int flag = atoi(Buffer);
    printf("%d / %d is %d with a remainder of %d\n", temp, flag, (temp/flag),
       (temp % flag));
    // increment the count 
    count++;
  }
}
