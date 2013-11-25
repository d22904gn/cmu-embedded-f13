/*
 * rot13_arg.c: ROT13 cipher test application, with argument support
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    02 Nov 2013
 */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// Fix a buffer size.
#define BUF_SIZE 64

// Character code for our exit character.
#define EXIT_CHAR 3

// Applies ROT13 encoding to the input char array in-place.
void apply_ROT13(char *input, unsigned int len) {
    unsigned int i;

    for (i = 0; i < len; i++) {
        if ((65 <= input[i] && input[i] <= 77) ||
            (97 <= input[i] && input[i] <= 109)) {
            input[i] += 13;
        } else if ((78 <= input[i] && input[i] <= 90) ||
                   (110 <= input[i] && input[i] <= 122)) {
            input[i] -= 13;
        }
    }
}

// Writes a string to standard out.
void write_string(const char* buffer, unsigned int len) {
    // Loop and write until we have finished writing everything in
    // the buffer.
    int write_size;
    
    if (len != 0) {
        do {
            if ((write_size = write(STDOUT_FILENO, buffer, len)) < 0) {
                return;
            }
            len -= write_size;
        } while (len > 0);
    }
}

int main(int argc __attribute((unused)),
         char** argv __attribute((unused))) {
    char buf[BUF_SIZE];
    int read_size;
    int str_len, i;
    
    // Output command line arguments.
    
    // -- First as plain text.
    if (argc > 1) write_string("Args in plaintext:  ", 20);
    for (i = 1; i < argc; i++) {
        // Calculate the string length.
        str_len = 0;
        while (argv[i][str_len] != 0) {
            str_len++;
        }
    
        // Write the plain text string.
        write_string(argv[i], str_len);
        write_string(" ", 1);
    }
    
    // -- Then as ciphertext.
    if (argc > 1) write_string("\nArgs in ciphertext: ", 21);
    for (i = 1; i < argc; i++) {
        // Calculate the string length.
        str_len = 0;
        while (argv[i][str_len] != 0) {
            str_len++;
        }
    
        // Write the ciphertext.
        apply_ROT13(argv[i], str_len);
        write_string(argv[i], str_len);
        write_string(" ", 1);
    }
    if (argc > 1) write_string("\n\n", 2);
    
    // Start encrypting from stdin.
    write_string("Encrypting from standard input (Type Ctrl-C + Enter to exit):\n", 62);
    while (1) {
        // Reset input size.
        read_size = 0;

        // Read input from stdin until we reach EOF (read_size = 0)
        do {
            if ((read_size = read(STDIN_FILENO, buf, BUF_SIZE)) < 0)
                return 1;
            
            // Check if user wants to exit.
            if (buf[0] == EXIT_CHAR) return 0;
            
            // Encrypt input
            apply_ROT13(buf, read_size);
            
            // Write to stdout what we have so far.
            if (read_size != 0) write_string(buf, read_size);
        } while (read_size != 0);
        
        write_string("\n", 1);
    }
}
