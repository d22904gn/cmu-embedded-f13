/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    08 Nov 2013
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv)
{
    int i = 0;
    // Symbols for turning cursor
    int sym_size = 4;
    char sym_arr[sym_size];
    sym_arr[0] = '|';
    sym_arr[1] = '/';
    sym_arr[2] = '-';
    sym_arr[3] = '\\';
    
    // Overwritten Element on first iteration
    printf("%c",sym_arr[0]);
    
    while(1) {
        // Move cursor backward and write next char
        printf("\b%c",sym_arr[i]);
        i++;
        if (i >= sym_size) {
            i = i % sym_size;
        }
        sleep(200);
    }
    
	return 0;
}

