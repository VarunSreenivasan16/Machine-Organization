////////////////////////////////////////////////////////////////////////////////
// Main File:        (mem.c)
// This File:        (mem.c)
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"

/*
 * This structure serves as the header for each allocated and free block
 * It also serves as the footer for each free block
 * The blocks are ordered in the increasing order of addresses 
 */
typedef struct blk_hdr {                         
        int size_status;
  
    /*
    * Size of the block is always a multiple of 8
    * => last two bits are always zero - can be used to store other information
    *
    * LSB -> Least Significant Bit (Last Bit)
    * SLB -> Second Last Bit 
    * LSB = 0 => free block
    * LSB = 1 => allocated/busy block
    * SLB = 0 => previous block is free
    * SLB = 1 => previous block is allocated/busy
    * 
    * When used as the footer the last two bits should be zero
    */

    /*
    * Examples:
    * 
    * For a busy block with a payload of 20 bytes (i.e. 20 bytes data + an additional 4 bytes for header)
    * Header:
    * If the previous block is allocated, size_status should be set to 27
    * If the previous block is free, size_status should be set to 25
    * 
    * For a free block of size 24 bytes (including 4 bytes for header + 4 bytes for footer)
    * Header:
    * If the previous block is allocated, size_status should be set to 26
    * If the previous block is free, size_status should be set to 24
    * Footer:
    * size_status should be 24
    * 
    */
} blk_hdr;

/* Global variable - This will always point to the first block
 * i.e. the block with the lowest address */
blk_hdr *first_blk = NULL;

/*
 * Note: 
 *  The end of the available memory can be determined using end_mark
 *  The size_status of end_mark has a value of 1
 *
 */

/* 
 * Function for allocating 'size' bytes
 * Returns address of allocated block on success 
 * Returns NULL on failure 
 * Here is what this function should accomplish 
 * - Check for sanity of size - Return NULL when appropriate 
 * - Round up size to a multiple of 8 
 * - Traverse the list of blocks and allocate the best free block which can accommodate the requested size 
 * - Also, when allocating a block - split it into two blocks
 * Tips: Be careful with pointer arithmetic 
 */
void* Mem_Alloc(int size) {                      
    // Your code goes in here
     
    // Sanity check
    if (size <= 0) {
      return NULL;
    }
        
    // increment to account for header
    int rounded_Size = size + 4;
    
    // Round up to multiple of 8
    if ((rounded_Size % 8)!= 0) {
       rounded_Size += (8 - (rounded_Size % 8));
    }   

    // stores header value
    int header_Val;

    // pointer used to traverse 
    blk_hdr* curr = first_blk;

    // pointer used to store best location
    blk_hdr* best = first_blk;

    // stores blocksize
    int blockSize = 0;

    // counter
    int bestFit = 0;

    // used to compare
    int bestFitVal = 0;

    // traverse
    while (1) {
       // store header value
       header_Val = (curr -> size_status);
              
       // check if end mark is reached
       if (header_Val == 1) {
          break;
       }
           
       blockSize = (header_Val >> 2) << 2;
      
       // update best
       if (bestFit == 0 && (header_Val & 1) == 0 && 
              blockSize >= rounded_Size) {
          // check if perfect match and break
          if (blockSize == rounded_Size) {
             best = curr;
             bestFit = 1;
             break;

            // set best temporarily and continue traversing
          } else if (blockSize > rounded_Size) {
            best = curr;
            bestFit = 1;
            bestFitVal = blockSize;
          }  

         // update best
       } else if (bestFit == 1 && (header_Val & 1) == 0  && 
                blockSize >= rounded_Size &&
                       blockSize < bestFitVal) {
          // break if perfect match
          if (blockSize == rounded_Size) {
             best = curr;
             break;

            // set new best value and continue traversing
          } else if (blockSize > rounded_Size) {
             best = curr;
             bestFitVal = blockSize;
          }
       }

       // update current position
       curr += (blockSize/sizeof(blk_hdr));
    }

    // check for a fit
    if (bestFit == 0) {
       return NULL;
    }
    
    // attempt to split
    header_Val = (best -> size_status);
    blockSize = (header_Val >> 2) << 2;
    
    // check for split
    if ((blockSize - rounded_Size) >= 8) {      
       // pointer to best
       blk_hdr* allocBlock = best;

       // pointer to free block
       blk_hdr* freeBlock = best + (rounded_Size/sizeof(blk_hdr));
      
       // free block size
       int freeBlockSize = blockSize - rounded_Size;
       
       // pointer to free block footer
       blk_hdr* freeBlockFooter = freeBlock + 
                 ((freeBlockSize - 4)/sizeof(blk_hdr));

       // Update header for allocated Block 
       (allocBlock -> size_status) = rounded_Size + (header_Val & 2) + 1;

       // create header for free block
       (freeBlock -> size_status) = freeBlockSize + 2;

       // create footer for free block
       (freeBlockFooter -> size_status) = freeBlockSize;

    } else { 
       // can't split
       // update 'a' bit for allocated block
        blk_hdr* allocatedBlock = best;
       (allocatedBlock -> size_status) = header_Val + 1;

       // update p bit for next block
       blk_hdr* nextBlock = allocatedBlock + (blockSize/sizeof(blk_hdr));
       (nextBlock -> size_status) = (nextBlock -> size_status) + 2; 
    }
    return (best + 1);
}
      


/* 
 * Function for freeing up a previously allocated block 
 * Argument - ptr: Address of the block to be freed up 
 * Returns 0 on success 
 * Returns -1 on failure 
 * Here is what this function should accomplish 
 * - Return -1 if ptr is NULL
 * - Return -1 if ptr is not 8 byte aligned or if the block is already freed
 * - Mark the block as free 
 * - Coalesce if one or both of the immediate neighbours are free 
 */
int Mem_Free(void *ptr) {                        
    // Your code goes in here 

     // check if ptr is NULL
     if (ptr == NULL) {
        return -1;
     }
          
     // block to be freed
     blk_hdr* block = ptr - 4;

     // obtain size_status of block to be freed
     int blockSize_Status = (block -> size_status);
     
     // derive block size
     int blockSize = (blockSize_Status >> 2) << 2;
     
     // check for 8 byte alignment
     if ((blockSize % 8) != 0) {
        return -1;
     } 

     // check if already freed
     if ((blockSize_Status & 1) == 0) {
        return -1;
     }

     // mark the block as free
     block -> size_status = block->size_status - 1;

     // attempt to coalesce with next block
     // address of block located after the block to be freed
     blk_hdr* addrOfNextBlock = block + (blockSize/ sizeof(blk_hdr)); 

     // size_status of next block
     int sizeStatusOfNextBlock = (addrOfNextBlock -> size_status); 

     // size of next block
     int sizeOfNextBlock = (sizeStatusOfNextBlock >> 2) << 2; 
      
     if (((sizeStatusOfNextBlock & 1) == 0) && (sizeStatusOfNextBlock != 1)) {
        // next block is free

        // modify next block's footer
        blk_hdr* nextBlockFooter = addrOfNextBlock + 
                        ((sizeOfNextBlock - 4) / sizeof(blk_hdr));
        nextBlockFooter -> size_status = sizeOfNextBlock + blockSize;

        // modify block header
        block -> size_status += sizeOfNextBlock;

        // blocksize is updated
        blockSize += sizeOfNextBlock;

     } else {
         // can't coalesce         
     
         // create footer
         blk_hdr* footerAddress = block + ((blockSize - 4)/ sizeof(blk_hdr));
         footerAddress -> size_status = blockSize;

         // modify p bit of next block header
         addrOfNextBlock -> size_status = sizeStatusOfNextBlock - 2;       
     }

    
     // attempt to coalesce with previous block 
     if (((blockSize_Status & 2) == 0) && (block != first_blk)) {
        // obtain size of previous block
        int sizeOfPrevBlock = (block - 1) -> size_status;

        // obtain starting address of previous block
        blk_hdr* addrOfPrevBlock = block - (sizeOfPrevBlock / sizeof(blk_hdr));

        // obtain size_status of previous block and update
        int size_StatusOfPrevBlock = (addrOfPrevBlock -> size_status);
        addrOfPrevBlock -> size_status = size_StatusOfPrevBlock + blockSize;

        // block footer
        blk_hdr* footer = block + ((blockSize - 4)/ sizeof(blk_hdr));
        footer -> size_status = sizeOfPrevBlock + blockSize;
       
     } else {
         // can't coalesce with prev block
         blk_hdr* footer = block + ((blockSize - 4)/ sizeof(blk_hdr)); 
         footer -> size_status = blockSize; 
         // modify p bit of next block's header
         addrOfNextBlock -> size_status = sizeStatusOfNextBlock - 2;
     }
     return 0;
}


/*
 * Function used to initialize the memory allocator
 * Not intended to be called more than once by a program
 * Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated
 * Returns 0 on success and -1 on failure 
 */
int Mem_Init(int sizeOfRegion) {                         
    int pagesize;
    int padsize;
    int fd;
    int alloc_size;
    void* space_ptr;
    blk_hdr* end_mark;
    static int allocated_once = 0;
  
    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: Mem_Init has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 
                    fd, 0);
    if (MAP_FAILED == space_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }
  
     allocated_once = 1;

    // for double word alignement and end mark
    alloc_size -= 8;

    // To begin with there is only one big free block
    // initialize heap so that first block meets 
    // double word alignement requirement
    first_blk = (blk_hdr*) space_ptr + 1;
    end_mark = (blk_hdr*)((void*)first_blk + alloc_size);
  
    // Setting up the header
    first_blk->size_status = alloc_size;

    // Marking the previous block as busy
    first_blk->size_status += 2;


    // Setting up the end mark and marking it as busy
    end_mark->size_status = 1;

    // Setting up the footer
    blk_hdr *footer = (blk_hdr*) ((char*)first_blk + alloc_size - 4);
    footer->size_status = alloc_size;
  
    return 0;
}

/* 
 * Function to be used for debugging 
 * Prints out a list of all the blocks along with the following information i
 * for each block 
 * No.      : serial number of the block 
 * Status   : free/busy 
 * Prev     : status of previous block free/busy
 * t_Begin  : address of the first byte in the block (this is where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block (as stored in the block header) (including the header/footer)
 */ 
void Mem_Dump() {                        
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end = NULL;
    int t_size;

    blk_hdr *current = first_blk;
    counter = 1;

    int busy_size = 0;
    int free_size = 0;
    int is_busy = -1;

    fprintf(stdout, "************************************Block list***\
                    ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
                    --------------------------------\n");
  
    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;
    
        if (t_size & 1) {
            // LSB = 1 => busy block
            strcpy(status, "Busy");
            is_busy = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_busy = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "Busy");
            t_size = t_size - 2;
        } else {
            strcpy(p_status, "Free");
        }

        if (is_busy) 
            busy_size += t_size;
        else 
            free_size += t_size;

        t_end = t_begin + t_size - 1;
    
        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status, 
        p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);
    
        current = (blk_hdr*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
                    ------------------------------\n");
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fprintf(stdout, "Total busy size = %d\n", busy_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", busy_size + free_size);
    fprintf(stdout, "***************************************************\
                    ******************************\n");
    fflush(stdout);

    return;
}
