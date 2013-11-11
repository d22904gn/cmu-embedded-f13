/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    08 Nov 2013
 */

#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    char symbols[] = {'|', '/', '-', '\\'};
    int i = 1;
    
    putchar('|');
    sleep(200);
    
    while (1) {
        putchar('\b');
        putchar(symbols[i]);
        
        if (++i == 4) i = 0;
        sleep(200);
    }
    
	return 0;
}

