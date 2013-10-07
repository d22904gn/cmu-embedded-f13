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
// Memory complexity: O(1)
// Time complexity: O(n^2)
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

// ****************************************************************************
// ***** Memory/Space improvements and tradeoffs:
// ****************************************************************************
// Memory complexity: O(n)
// Time complexity: O(n)
// This optimization decreases the time complexity from O(n^2) to O(n) but
// increases the memory complexity from O(1) to O(n).
// ****************************************************************************
// ***** Algorithm:
// ****************************************************************************
// A count array of size (n+1) is created to keep track of the number of
// appearances of all the numbers in the array. The number which has a count of
// only 1 is the unique element.
int oddball(int *arr, int len) {
    // Changed counter type to unsigned int; then compiler is free to
    // assume the overflows never happen.
    unsigned int i = len - 1;
    unsigned int num_uniq = (len + 1)/2;
    unsigned int count[num_uniq + 1];

    // Use a do-while loop instead of for loop since loop guaranteed to run at
    // least once. Also counted down instead of counting upwards and changed
    // comparison to != instead of >=. This avoids initial compare and
    // conditional branch before loop starts.
    // Loops at most 2n-1 times.
    do {
        count[arr[i]]++;
        i--;
    } while (i != 0);

    do {
        // Found the unique element.
        if (count[num_unique] == 1) {
            return num_unique
        }
        num_uniq--;
    } while (num_unique != 0);
    return 0;
}

#endif

#ifdef OPTIMIZE2
// Mark function as pure
//__pure int oddball(int *arr, int len);

// ****************************************************************************
// ***** Memory/Space improvements and tradeoffs:
// ****************************************************************************
// Memory complexity: O(1)
// Time complexity: O(n)
// This optimization decreases the time complexity from O(n^2) to O(n) and keeps
// the memory complexity constant at O(1)
// ****************************************************************************
// ***** Algorithm:
// ****************************************************************************
// Given an array of elements where every element appears an even number of
// times except for an element N, XOR-ing every element in the array together
// will result in N. This is due to the following principles:
//      1) XOR is commutative   -> Given an example array [42 2 3 2 3] where 42
//                                 is the unique element:
//                                 42 ^ 2 ^ 3 ^ 2 ^ 3 = 42 ^ (2 ^ 2) ^ (3 ^ 3)
//      2) a XOR a = 0          -> The elements appearing an even number of
//                                 times will just cancel out to 0.
//                                 42 ^ (2 ^ 2) ^ (3 ^ 3) = 42 ^ (0 ^ 0)
//                                                        = 42 ^ 0
//      3) a XOR 0 = 0          -> With all the other even count elements
//                                 cancelling out to 0, the result will be
//                                 N XOR 0 = N
//                                 Hence: 42 ^ 2 ^ 3 ^ 2 ^ 3 = 42 ^ 0 = 42
//                                        => 42 is the unique element.
int oddball(int *arr, int len) {
    // Changed counter type to unsigned int; then compiler is free to
    // assume the overflows never happen.
    unsigned int i = len - 1;
    unsigned int result = 0;

    // Use a do-while loop instead of for loop since loop guaranteed to run at
    // least once. Also counted down instead of counting upwards and changed
    // comparison to != instead of >=. This avoids initial compare and
    // conditional branch before loop starts.
    // Loops at most 2n-1 times.
    do {
        result ^= arr[i];
        i--;
    } while (i != 0);
    return result;
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




