/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// Fix a buffer size.
#define BUF_SIZE 64

// Applies ROT13 encoding to the input char array in-place.
int applyROT13(char *input, unsigned int len) {
    int i;
    
    for (i = 0; i < len; i++) {
        if ((65 <= input[i] && input[i] <= 77) ||
            (97 <= input[i] && input[i] <= 109)) {
            input[i] += 13;
        } else if ((78 <= input[i] && input[i] <= 90) ||
                   (110 <= input[i] && input[i] <= 122)) {
            input[i] -= 13;
        }
    }
    
    return 0;
}

int main(void) {
    char buf[BUF_SIZE];
    int inputSize;
    
    while (1) {
        // Reset input size.
        inputSize = 0;
        
        // Read input from stdin.
        if ((inputSize = read(STDIN_FILENO, buf, BUF_SIZE)) < 0) {
            exit(1);
        }
        
        // Do ROT13
        applyROT13(buf, inputSize);
        
        // Write to stdout.
        if (write(STDOUT_FILENO, buf, inputSize) < 0) {
            exit(1);
        }
    }
}
