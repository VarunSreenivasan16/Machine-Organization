////////////////////////////////////////////////////////////////////////////////
// Main File:        (verify_hetero.c)
// This File:        (verify_hetero.c)
// Other Files:      (None)
// Semester:         CS 354 Spring 2018
//
// Author:           (Varun Sreenivasan)
// Email:            (vsreenivasan@wisc.edu)
// CS Login:         (sreenivasan)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
    int size;
    int **array;
} Square;

Square * construct_square(char *filename);
int verify_hetero(Square * square);
void insertion_sort(int* arr, int size);

/* main function 
 * 
 * (argc) number of arguments 
 * (argv) pointer to an array containg arguments
 * returns 0
 */
int main(int argc, char *argv[])                        
{
    // Check input arguments to get filename
    // check if there are sufficient arguments
    if(argc != 2)
    {
      perror("Insufficient arguments.");
      exit(1);
    }
    else
    {	
        // file is a pointer to array containing 2nd argument
       char* file = *(argv + 1);
    
        // Construct square
       Square* square = construct_square(file);

       // Verify if it's a heterosquare and print true or false
       int check =  verify_hetero(square);
        
       //free allocated memory
       for(int i = 0; i < (square -> size); i++)
       {
          free( *((square -> array) + i));
       }

       free((square -> array));
       free(square);
 
       //print statements
       if(check != 0)
       {
         printf("%s\n", "true");
       }   
        else
       {
         printf("%s\n", "false");
       }

    }

    return 0;
}

/* construct_square reads the input file to initialize a square struct
 * from the contents of the file and returns the square.
 * The format of the file is defined in the assignment specifications
 *
 * (filename) pointer to the file's name
 * returns square struct
 */
Square * construct_square(char *filename)                
{

    // Open and read the file
    FILE *inputFile = fopen(filename, "r");

    //allocate memory for square struct
    Square* temp = malloc(sizeof(Square));

    //check for error
    if(temp == NULL)
    {
        perror("malloc error");
        exit(1);
    }

    //size of square
    temp -> size = 0;
    
    //check if file can be opened
    if(inputFile == 0)
    {
       perror("Cannot open file for reading.");
       exit(1);
    }
    else
    {
       
       // Read the first line to get the square size
       const int max_Length = 101;
       char str[max_Length];
       fgets(str, max_Length, inputFile);
       temp -> size = atoi (str);
       
       // Initialize a new Square struct of that size
       temp -> array = malloc(sizeof(int*)*(temp -> size));
        
       //check for error
       if((temp -> array)== NULL)
       {
           perror("malloc error");
           exit(1);
       }
        
       //create an array of arrays of memory
       for(int i = 0; i < (temp -> size); i++)
       {
          //allocate memory
          *((temp -> array) + i) = malloc(sizeof(int)* (temp -> size));
         
          //check for error
          if((*((temp -> array) + i)) == NULL)
          {
             perror("malloc error");
             exit(1);
          }
 
        }

       
        
	 // Read the rest of the file to fill up the square 
        for(int i = 0; i < (temp -> size); i++)
        { 
             //obtain each line
             fgets(str, max_Length, inputFile);
          
             //use tokenizer to remove comma
             const char s[2] = ",";
  
             //pointer to value
             char *token;
             token = strtok(str,s);
             
	     //stores value that is converted to int
             int tem = 0; 
  
             //counter to assign value to array
             int j = 0;
             
             //iterate till all commas are removed from the line
             while(token != NULL)
             {
                //stores converted int value
                tem = atoi(token);
                
                //store value in array
                *(*((temp -> array) + i)+ j) = tem;
                token = strtok(NULL,s);
                j++;            
     
             }
          
        }

        //close file
        fclose(inputFile);
       
    }
       
    //return instance of struct
    return temp;

}

/* verify_hetero verifies if the square is a heterosquare
 * 
 * (square) instance of Square struct
 * returns 1(true) or 0(false)
 */
int verify_hetero(Square * square)               
{
    // Calculate sum of the following and store it in an array
    // all rows and cols
    // main diagonal
    // secondary diagonal

    //store size of square
    int sqSize = (square -> size);

    //allocate memory for 1D array
    int * arr = malloc(sizeof(int) * (2 * sqSize + 2) );

    //check for error
    if(arr == NULL)
    {
      perror("malloc error");
      exit(1);
    }

    //Sum of Rows
    for(int i = 0; i < sqSize; i++)
    {
       int countRows = 0;
       for(int j = 0; j < sqSize; j++)
       {
         countRows +=  * ( *( (square -> array) + i) + j);
       }

       *(arr + i) = countRows;
      
    }
    
    //Sum of Cols
    for(int j = 0; j < sqSize; j++)
    {
       int countCols = 0;

       for(int i = 0; i < sqSize; i++)
       {
         countCols +=  * ( *( (square -> array) + i) + j);
       }

       *(arr + sqSize + j) = countCols;
      
    }
    
    //Sum of diagonal 1
    int q = 0;
    int r = 0;
    int diag1 = 0;

    while((q < sqSize) && (r < sqSize))
    {
   	diag1 += * ( *( (square -> array) + q) + r);
        q++;
        r++;
    }

    *(arr + 2*sqSize) = diag1;
   

    //Sum of diagonal 2
    int m = 0;
    int n = sqSize - 1;
    int diag2 = 0;

    while((m < sqSize) && (n >= 0))
    {
      diag2 += * ( *( (square -> array) + m) + n);
      m++;
      n--;
    }

    *(arr + 2*sqSize + 1) = diag2;
 

    // Pass the array to insertion_sort function
    insertion_sort(arr, 2*sqSize + 2);
   
    
    // Check the sorted array for duplicates
    int check = 1;

    for(int i = 0; i < 2*sqSize + 1; i++)
    {
 	if(*(arr +i) == *(arr + i + 1))
        {
           check = 0;
           break;
        }
    }

    //free allocated memory
    free(arr);
 
    return check;

}

/* insertion_sort sorts the arr in ascending order
 * 
 * (arr) pointer to array
 * (size) size of array
 */
void insertion_sort(int* arr, int size)                  
{
    // Sort the arr
    int i;
    int val;
    int j;

    for (i = 1; i < size; i++)
    {
       val = *(arr + i);
       j = i-1;
 
       /* Move elements that are
          greater than val,  one position ahead
          of their current position */
       while (j >= 0 && *(arr + j) > val)
       {
           *(arr + j + 1) = *(arr + j);
           j = j-1;
       }

       *(arr + j + 1) = val;
    }

}
