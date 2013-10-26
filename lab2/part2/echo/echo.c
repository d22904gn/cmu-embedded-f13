/*
 * echo.c: Echo program with own libc
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    20 Oct 2013
 */

#include <stdlib.h>
#include <unistd.h>

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

int main(int argc, char** argv) {
    int i = 1;
    int arg_len = 0;
    
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
    
    // Write a newline before exiting.
    write_string("\n", 1);
    
    return 0;
}
