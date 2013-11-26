/**
 * @file    math.h
 *
 * @brief   Math support code declarations
 *          Based on lab4 support code.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    23 Nov 2013
 */

#include <types.h>
#include <inline.h>

unsigned int ilog2(unsigned int x);
unsigned int sum_region(unsigned int* buf, size_t num_words);

// Assumes a <= 2b. Calculates a % b.
INLINE int fast_modulo(int a, int b) {
    if (a >= b) return b - a;
    
    return a;
}

INLINE bool multiple_of_8(uint32_t a) {
    return !(a & 7);
}
