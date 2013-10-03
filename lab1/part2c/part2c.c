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
__pure int oddball(int *arr, int len) {
	/* Put your code here */
    int i, j;
    int foundInner;
    int result = 0;

    // Used a loop that counted down instead of counting upwards.
    for (i = len-1; i >= 0; i--) {
        foundInner = 0;
        for (j = len; j >= 0; j--) {
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

#ifdef OPTIMIZE2
// Mark function as pure
__pure int oddball(int *arr, int len);

// Final optimized version.
int oddball(int *arr, int len) {
    // Changed counter type to unsigned int.
    unsigned int i = len - 1, j = len;
    
    // Used a do-while loop instead of for loop since loop guaranteed to
    // run at least once. Also counted down instead of counting upwards.
    // and changed comparison to != instead of >=.
    
    // TODO: Think about cache misses; counting down implies that we cannot
    // use spatial locality. Might want to check from 0 instead. So maybe
    // use something like newcounter = len - j.
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




