/*
 * echo.c: Echo program with own libc
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    20 Oct 2013
 */

#include <unistd.h>

int main(int argc, char** argv) {
    int i = 1;
    int arg_len = 0;
    
    while (i < argc) {
        // Obtain argument length
        arg_len = 0;
        while (argv[i][arg_len] != 0) {
            arg_len++;
        }
        
        write(STDOUT_FILENO, argv[i], arg_len);
        write(STDOUT_FILENO, " ", 1);
        
        i++;
    }
    
    // Write a newline
    write(STDOUT_FILENO, "\n", 1);
    
    return 0;
}
