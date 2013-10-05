#include <stdlib.h>
#include <stdio.h>

/**
 * Function: oddball
 * Description: Baseline implementation for finding the number that
 *   occurs only once in the array.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
#ifdef DEFAULT
int oddball(int *arr, int len) {
	int i, j;
	int foundInner;
	int result = 0;

	for (i = 0; i < len; i++) {
		foundInner = 0;
		for (j = 0; j < len; j++) {
			if (i == j) {
				continue;
			}
			if (arr[i] == arr[j]) {
				foundInner = 1;
			}
		}
		if (foundInner != 1) {
			result = arr[i];
		}
	}

	return result;
}
#endif

#ifdef OPTIMIZE1
// Mark function as pure
//__pure int oddball(int *arr, int len);

int oddball(int *arr, int len) {
    // Changed counter type to unsigned int; then compiler is free to
    // assume the overflows never happen.
    unsigned int i = len - 1, j = len - 1;
    
    // Use a do-while loop instead of for loop since loop guaranteed to run at
    // least once. Also counted down instead of counting upwards and changed
    // comparison to != instead of >=. This avoids initial compare and
    // conditional branch before loop starts.
    do {
		do {
			// Removed redundant if statement and merged the conditional check
            // into the second if statement.
            if (i != j && arr[i] == arr[j]) {
				// Removed foundInner variable and result variable. Just
                // immediately return the result once we find it.
                return arr[i];
			}
			
			j--;
		} while (j != 0);
		
        i--;
    } while (i != 0);
	
    return 0;
}
#endif

#ifdef OPTIMIZE2
// Mark function as pure
//__pure int oddball(int *arr, int len);

// Algorithm uses an in-place, iterative quicksort to sort array, and then
// it searches linearly for the unique element. Runtime complexity O(n log n).
// An in-place, iterative sort was chosen to avoid too many stack accesses
// (GCC seems to like to save and use callee saved registers in their function
// prologue even if it is possible to execute the code by only using the
// argument registers) and also to take advantage of spatial and temporal
// locality in the cache.

// Quicksort bit based on iterative quicksort code from
// http://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Quicksort#Iterative_Quicksort
// Note that we do not use a random pivot; division and modulo is too slow.
#define STACK_SIZE 32

int oddball(int *arr, int len) {
    // Quicksort variables
    unsigned int lower = 0, greaterIdx = 0, pos = 0;
    unsigned int stack[STACK_SIZE];
    unsigned int upper = len;
    int pivot = 0, temp = 0;
    
    // Linear search variables
    unsigned int i = len - 2;
    
    while (1) {
        // Sort from lower to upper - 1
        while (lower + 1 < upper) {
            // Check for stack overflow.
            if (pos == STACK_SIZE) {
                pos = 0;
                upper = stack[0];
            }
            
            pivot = arr[lower]; // Pick pivot
            stack[pos++] = upper; // Mark right partition for later
            
            // Partition array and start sorting wrt pivot.
            greaterIdx = lower - 1; // OK to unsigned overflow.
            while (1) {
                // Search for greater, then smaller element.
                while (arr[++greaterIdx] < pivot);
                while (pivot < arr[--upper]);
                
                // Check if array partition has been completely sorted.
                if (greaterIdx >= upper) break;
                
                // Swap elements.
                temp = arr[greaterIdx];
                arr[greaterIdx] = arr[upper];
                arr[upper] = temp;
            }
            
            upper++;
        }
        
        // We are done when the stack is empty.
        if (pos == 0) break; 
        
        // Else, continue sorting
        lower = upper;
        upper = stack[--pos];
    }
    
    // Linear search bit.
    do {
        temp = arr[i];
        if (temp != arr[i + 1] && temp != arr[i - 1]) return temp;
        i--;
    } while (i != 1);
    
    return 0;
}
#endif

/**
 * Function: randGenerator
 * Description: Generate a random array that is in compliance with
 *   lab specification
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   Unused, but feel free to utilize it.
 */

int randGenerator(int *arr, int len) {
	int i, j, r, rcount;
	for (i = 0; i < len; i++) {
		do {
			rcount = 0;
			r = rand()%(len/2 + 1) + 1;
			for (j = 0; j < i && rcount < 2; j++) {
				if (arr[j] == r) {
					rcount++;
				}
			}
		} while(rcount >= 2);
		arr[i] = r;
		printf("%d ", r);
	}
	printf("\nDone generating\n");
	return 0;
}




