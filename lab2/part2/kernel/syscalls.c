/*
 * syscalls.c: Kernel system calls.
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <bits/fileno.h>
#include <bits/errno.h>
#include <exports.h>
#include "common.h"
#include "syscalls.h"

// Memory ranges
#define RAM_START   0xa0000000u
#define RAM_END     0xa3ffffffu
#define ROM_START   0x00000000u
#define ROM_END     0x00ffffffu

// Special characters
#define EOT         4u
#define BACKSPACE   8u
#define DELETE      127u
#define NEWLINE     10u
#define CR          13u

ssize_t read(int fd, void *buf, size_t count) {
    // Temp local vars
    uchar c;
    uint read_count = 0;
    
    // Re-cast buf into char pointer.
    uchar *buffer = (uchar *) buf;
    
    // For readability
    uint buffer_start = (uint) buffer;
    uint buffer_end = ((uint) buffer) + count - 1; 
    
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

ssize_t write(int fd, const void *buf, size_t count) {
    // Temp local vars
    uint write_count = 0;
    
    // Re-cast buf into char pointer.
    uchar *buffer = (uchar *) buf;
    
    // For readability
    uint buffer_start = (uint) buffer;
    uint buffer_end = ((uint) buffer) + count - 1; 
    
    // Sanity checks.
    if (fd != STDOUT_FILENO)
        return -EBADF;
    if (!((buffer_start >= ROM_START && buffer_end <= ROM_END) ||
          (buffer_start >= RAM_START && buffer_end <= RAM_END)))
        return -EFAULT;
    
    while (write_count != count) {
        putc(*buffer++);
        write_count++;
    }
    
    return write_count;
}
