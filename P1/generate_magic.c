////////////////////////////////////////////////////////////////////////////////
// Main File:        (generate_magic.c)
// This File:        (generate_magic.c)
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

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
    int size;
    int **array;
} Square;

int get_square_size();
Square * generate_magic(int size);
void write_to_file(char *filename, Square * square);

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
   
     // file is a pointer to array containing 2nd argument
    char* file = *(argv + 1);
 
    // Get size from user
    int size = get_square_size();

    // Generate the magic square
    Square* square = generate_magic(size);

    // Write the square to the output file    
    write_to_file(file, square);

    //free allocated memory
    for(int i = 0; i < (square -> size); i++)
    {
       free( *((square -> array) + i));
    }

    free((square -> array));
    free(square);

 
    return 0;
}

/* get_square_size prompts the user for the magic square size
 * checks if it is an odd number >= 3 and returns the number
 * returns (size of square)
 */
int get_square_size()            
{
    //stores square size
    int number = 0;

    printf("%s\n","Enter size of magic square, must be odd");

    //obtain input
    scanf("%d", &number);

    //check the number
    if(((number % 2) != 1) ||  (number < 3))
    {
      printf("%s\n","Size must be an odd number >= 3.");
      exit(1);

    }
    
    return number;
}

/* generate_magic constructs a magic square of size n
 * using the Siamese algorithm and returns the Square struct
 *
 * (n) - size of square
 * returns (instance of square struct)
 *
 */
Square * generate_magic(int n)           
{
   //allocate memory 
   Square* temp = malloc(sizeof(Square));

   //check for error
   if(temp == NULL)
   {
        perror("malloc error");
        exit(1);
   }
 
   //assign size of square
   temp -> size = n;

   //allocate memory
   temp -> array = malloc(sizeof(int*) * n);
 
   //check for error  
   if((temp -> array)== NULL)
   {
       perror("malloc error");
       exit(1);
   }
  
   //allocate memory
   for(int i = 0; i < (temp -> size); i++)
   {
        
        *((temp -> array) + i) = malloc(sizeof(int)* (temp -> size));
      
       //check for error
       if((*((temp -> array) + i)) == NULL)
       {
          perror("malloc error");
          exit(1);
       }
   }

   
   //initialize all values in 2-D array to 0
   for(int i = 0; i < (temp -> size); i++)
   {
      for(int j = 0; j < (temp -> size); j++)
      {
         *( *(temp -> array + i ) + j) = 0;
      }
   }

 
   //counter of no.of values assigned
   int boxesFilled = 1;

   // x and y are coordinates of array
   int x = (((n+1)/2)-1);
   int y = n-1;

   //assign 1 to last column and middle row
   *( *(temp -> array + x) + y) = 1;

   //xPrev and yPrev store previous coordinates
   int xPrev = 0;
   int yPrev = 0; 

   //iterate till all points are filled
   while(boxesFilled < (n*n))
   {
      //store current coordinates in xPrev and yPrev
      xPrev = x;
      yPrev = y;

      //check if last row
      if(x == n-1)
      {
        //set current row to 0
        x = 0;

      }
      else
      {
        //increment row index
        x = x + 1;
      }

      //check if last column
      if(y == n-1)
      {
        //set current column index to 0
        y = 0;
      }
      else
      {
        //incremment column index
        y = y + 1;
      }
      
      //iterate till an unfilled point is found
      while( *( *(temp -> array + x ) + y) != 0)
      {
        //check if column is index 0
        if( y == 0)
        {
          // set current column to last
          y = n-1;
        }
        else
        {
          //decrement column index
          y = yPrev - 1;
          
        }
        
        //set x to its previous value
        x = xPrev;
      }
     
      //assign values and increment boxesFilled
      *( *(temp -> array + x ) + y) = ++boxesFilled;     
    
      
   }
   
   //return instance of square struct
   return temp;
}

/* write_to_file opens up a new file(or overwrites the existing file)
 * and writes out the square in the format expected by verify_hetero.c
 *
 * (filename)  pointer to file's name
 * (square) instance of square struct
 */
void write_to_file(char *filename, Square * square)              
{
    // Write the square to file
    FILE * outputFile = fopen(filename, "w");
    
    //print size
    fprintf(outputFile, "%d\n", (square -> size));
    
    //print all values
    for(int i = 0; i < (square -> size); i++)
    {
       for(int j = 0; j < (square -> size) - 1 ; j++)
       {
          fprintf(outputFile, "%d,",   *(*((square -> array)+i)  + j));
       }

       fprintf(outputFile, "%d\n", *(*((square -> array)+i)+(square -> size)-1));
    }
    
    //close file
    fclose(outputFile);
}
