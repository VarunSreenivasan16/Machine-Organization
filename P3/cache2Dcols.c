////////////////////////////////////////////////////////////////////////////////
// Main File:        csim.c
// This File:        cache2Drows.c
// Other Files:      cache1D.c and cache2Dcols.c (part 1) and csim.c (part 2)
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

int arr2D[3000][500];

//Main method for setting the value of each element in the array as the index
int main() {
    
   for (int col = 0; col < 500; col++) {
      for (int row = 0; row < 3000; row++) {
         arr2D[row][col] = row + col;
      }
   }
}

