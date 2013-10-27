/*
 * echo.c: Echo program with own libc
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    20 Oct 2013
 */

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

// Fix a buffer size.
#define BUF_SIZE 64

// Character code for our exit character.
#define EXIT_CHAR 3

// Writes a string to standard out.
void write_string(char* buffer, unsigned int len) {
    // Loop and write until we have finished writing everything in
    // the buffer.
    int write_size;
    
    if (len != 0) {
        do {
            if ((write_size = write(STDOUT_FILENO, buffer, len)) < 0) {
                exit(1);
            }
            len -= write_size;
        } while (len > 0);
    }
}

int main(int argc, char** argv) {
    char buf[BUF_SIZE];
    int read_size;
    unsigned int str_len, i;
    
    // Output command line arguments.
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
    if (argc > 1) write_string("\n\n", 2);
    
    // Start echoing from stdin.
    write_string("Echoing from standard input (Type Ctrl-C + Enter to exit):\n", 59);
    while (1) {
        // Reset input size.
        read_size = 0;
        
        // Read input from stdin until we reach EOF (read_size = 0)
        do {
            if ((read_size = read(STDIN_FILENO, buf, BUF_SIZE)) < 0) exit(1);
            
            // Check if user wants to exit.
            if (buf[0] == EXIT_CHAR) return 0;
            
            // Write to stdout what we have so far.
            if (read_size != 0) write_string(buf, read_size);
        } while (read_size != 0);
        
        write_string("\n", 1);
    }
}
