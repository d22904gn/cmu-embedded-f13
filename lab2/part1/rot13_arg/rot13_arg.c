/*
 * rot13_arg.c: ROT13 cipher - command line args version.
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    20 Oct 2013
 */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// Applies ROT13 encoding to the input char array in-place.
int apply_ROT13(char *input, unsigned int len) {
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

// Writes a string to stdout.
void write_string(char *str, unsigned int len) {
    int write_size = 0;
    
    do {
        if ((write_size = write(STDOUT_FILENO, str, len)) < 0) {
            exit(1);
        }
        len -= write_size;
    } while (len > 0);
}

int main(int argc, char **argv) {
    int i = 1;
    int arg_len = 0;
    
    // Print out arguments first.
    write_string("Plaintext: ", 11);
    while (i < argc) {
        // Obtain argument length
        arg_len = 0;
        while (argv[i][arg_len] != 0) {
            arg_len++;
        }
        
        // Write argument and space to stdout.
        write_string(argv[i], arg_len);
        write_string(" ", 1);
        
        i++;
    }
    
    write_string("\n", 1);
    
    // Now print out ROT13 applied to arguments.
    write_string("Ciphertext: ", 12);
    i = 1;
    while (i < argc) {
        // Obtain argument length
        arg_len = 0;
        while (argv[i][arg_len] != 0) {
            arg_len++;
        }
        
        // Apply ROT13 to argument.
        apply_ROT13(argv[i], arg_len);
        
        // Write argument and space to stdout.
        write_string(argv[i], arg_len);
        write_string(" ", 1);
        
        i++;
    }
    
    write_string("\n", 1);
    
    return 0;
}
