////////////////////////////////////////////////////////////////////////////////
// Main File:        intdate.c
// This File:        sendsig.c
// Other Files:      intdate.c, division.c
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
#include<string.h>
#include<signal.h>

/*
*main method
*arg - argc, *argv[]
*/
int main(int argc, char *argv[])
{
  // check for number of arguments
  if (argc != 3 ) 
  {
    perror("Usage: <signal type> <pid>\n");
    exit(1);
  }
  // obtain signal type
  char* signalType = *(argv + 1);
  // obtain pid
  int pid = atoi(*(argv + 2));
  
  // determine action
  if (strcmp("-i", signalType) == 0){
    kill(pid, SIGINT);
  }else if (strcmp("-u", signalType) == 0){
    kill(pid, SIGUSR1);
  }else{
    perror("Error: Signal not recognized");
    exit(1);
  }
}
