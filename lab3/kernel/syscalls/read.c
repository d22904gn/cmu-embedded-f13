/**
 * @file    read.c
 *
 * @brief   Read() system call.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    03 Nov 2013
 */

#include <bits/fileno.h>
#include <bits/errno.h>
#include <types.h>
#include <exports.h>
#include "io_constants.h"

ssize_t read(int fd, void *buf, size_t count) {
    // Temp local vars
    uchar c;
    uint32_t read_count = 0;
    
    // Re-cast buf into char pointer.
    uchar *buffer = (uchar *) buf;
    
    // For readability
    uint32_t buffer_start = (uint32_t) buffer;
    uint32_t buffer_end = ((uint32_t) buffer) + count - 1; 
    
    // Sanity checks.
    if (fd != STDIN_FILENO)
        return -EBADF;
    if (!(buffer_start >= RAM_START && buffer_end <= RAM_END))
        return -EFAULT;
    
    // Read chars from stdin until buffer is full.
    while (read_count != count) {
        c = (uchar) getc();
        
        switch (c) {
        case EOT:
            return read_count;
        case BACKSPACE:
        case DELETE:
            // Remove character from buffer.
            *buffer = 0;
            
            // Correct counters and offsets accordingly.
            if (read_count > 0) {
                buffer--;
                read_count--;
            }
        
            puts("\b \b");
            break;
        case NEWLINE:
        case CR:
            *buffer = '\n';
            read_count++;
            putc('\n');
            
            return read_count;
        default:
            putc(c);
            *buffer++ = c;
            read_count++;
        }
    }
    
    return read_count;
}
