/**
 * @file timer_test.c
 *
 * @brief Timer test suite.
 *
 * Links to libc.
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    10 Nov 2013
 */

#include <bits/types.h>
#include <unistd.h>
#include <stdio.h>

// Convenience typedefs and defines
#define CLOCK_FREQ 3250000
#define MS_PER_OVERFLOW ((uint32_t) ((UINT_MAX / CLOCK_FREQ) * 1000u))

typedef unsigned long ulong;

// Returns 1 if real value is within +- error of ideal value.
int is_acceptable(ulong real, ulong ideal, ulong error) {
    if (ideal + error <= real && ideal - error >= real) return 1;
    
    return 0;
}

int main(int argc, char** argv)
{
    int i;
    ulong start_time, end_time, wait_time;
    
    // Test time() sanity.
    printf("Testing time() sanity...");
    start_time = time();
    end_time = time();
    
    if (end_time < start_time) {
        printf("FAILED\n");
        printf("start_time: %lu, end_time: %lu\n", start_time, end_time);
        return -1;
    }
    printf("PASSED\n");
    
    // Test sleep() and time() sanity (+- 1ms)
    printf("Testing time() and sleep() sanity...");
    start_time = time();
    sleep(2000);
    end_time = time();
    
    if (is_acceptable(end_time, start_time + 2000, 1)) {
        printf("FAILED\n");
        printf("sleep(2000) -> start_time: %lu, end_time: %lu\n", start_time, end_time);
        return -1;
    }
    printf("PASSED\n");
    
    // Test drift (+- 1ms)
    printf("Testing drift...");
    start_time = time();
    for (i = 0; i < 1000; i++) {
        sleep(10);
    }
    end_time = time();
    
    if (is_acceptable(end_time, start_time + 10000, 1)) {
        printf("FAILED\n");
        printf("sleep(10) * 1000 -> start_time: %lu, end_time: %lu\n", start_time, end_time);
        return -1;
    }
    printf("PASSED\n");
    
    // Test overflow correctness. (+- 1ms)
    printf("Testing sleep() & time() overflow correctness (Will take 24+ minutes)...");
    wait_time = MS_PER_OVERFLOW * 1.1f;
    start_time = time();
    sleep(wait_time);
    end_time = time();
    
    if (end_time < MS_PER_OVERFLOW) {
        printf("FAILED\n");
        printf("sleep(%lu) -> start_time: %lu, end_time: %lu\n", wait_time, start_time, end_time);
        return -1;
    }
    if (is_acceptable(end_time, start_time + wait_time, 1)) {
        printf("FAILED\n");
        printf("sleep(%lu) -> start_time: %lu, end_time: %lu\n", wait_time, start_time, end_time);
        return -1;
    }
    printf("PASSED\n");
    
    // Test multiple overflow correctness.
    printf("Testing sleep() & time() multiple overflow correctness (Another 24+ minutes)...");
    wait_time = MS_PER_OVERFLOW * 1.1f;
    start_time = time();
    sleep(wait_time);
    end_time = time();
    
    if (end_time < 2 * MS_PER_OVERFLOW) {
        printf("FAILED\n");
        printf("sleep(%lu) -> start_time: %lu, end_time: %lu\n", wait_time, start_time, end_time);
        return -1;
    }
    if (is_acceptable(end_time, start_time + wait_time, 1)) {
        printf("FAILED\n");
        printf("sleep(%lu) -> start_time: %lu, end_time: %lu\n", wait_time, start_time, end_time);
        return -1;
    }
    printf("PASSED\n");
    
    printf("All tests PASSED!\n");
    
    return 0;
}
