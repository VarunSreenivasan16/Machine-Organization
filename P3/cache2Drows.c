////////////////////////////////////////////////////////////////////////////////
// Main File:        csim.c
// This File:        cache1D.c
// Other Files:      cache2Drows.c and cache2Dcols.c (part 1) and csim.c (part 2)
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

//Declare global array of integers of dimensions 3000 rows x 500 columns
int arr2D[3000][500];

//Main method 
int main() {

   for(int row = 0; row < 3000; row++) {
       for(int column = 0; column < 500; column++) {
           arr2D[row][column] = row + column;
       } 
   } 
}
