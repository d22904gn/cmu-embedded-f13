/**
 * @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */

#include <unistd.h>
#include <stdio.h>

// 255 char buffer. (Including null terminator.)
#define BUF_LEN 255

int main(int argc, char** argv)
{
	char buf[BUF_LEN];
    int read_size, sec, sec_deci;
    unsigned long start_time, total_time;
    
    while (1) {
        // Print prompt and record time.
        printf("> ");
        start_time = time();
        
        // Read input and forcibly null terminate.
        read_size = read(STDIN_FILENO, buf, BUF_LEN);
        if (read_size < 1) continue;
        buf[read_size - 1] = 0;
        
        // Calculate timing data. (printf has no float support -.-)
        total_time = time() - start_time;
        sec = total_time / 1000;
        sec_deci = (total_time - (sec * 1000)) / 100;
        
        // Output timing data.
        printf("%s\n%d.%ds\n", buf, sec, sec_deci);
    }
    
	return 0;
}
