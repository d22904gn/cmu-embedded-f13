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
#define RAM_START   0xa0000000
#define RAM_END     0xa3ffffff
#define ROM_START   0x00000000
#define ROM_END     0x00ffffff

// Special characters
#define EOT         4
#define BACKSPACE   8
#define DELETE      127
#define NEWLINE     10
#define CR          13

ssize_t read(int fd, void *buf, size_t count) {
    // Temp local vars
    char c;
    uint read_count = 0;
    
    // Re-cast buf into char pointer.
    char *buffer = (char *) buf;
    char *buffer_end = buffer + count - 1; // Readability
    
    // Sanity checks.
    if (fd != STDIN_FILENO)
        return -EBADF;
    if (!(buffer >= (char*) RAM_START && buffer_end <= (char*) RAM_END))
        return -EFAULT;
    
    // Read chars from stdin until buffer is full.
    while (read_count != count) {
        c = getc();
        
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
            puts("\n");
            
            return read_count;
        default:
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
    char *buffer = (char *) buf;
    char *buffer_end = buffer + count - 1; // Readability
    
    // Sanity checks.
    if (fd != STDOUT_FILENO)
        return -EBADF;
    if (!((buffer >= (char*) ROM_START && buffer_end <= (char*) ROM_END) ||
          (buffer >= (char*) RAM_START && buffer_end <= (char*) RAM_END)))
        return -EFAULT;
    
    while (write_count != count) {
        putc(*buffer++);
        write_count++;
    }
    
    return write_count;
}
