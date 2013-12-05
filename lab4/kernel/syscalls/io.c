/**
 * @file    io.c
 *
 * @brief   I/O related system calls.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    20 Nov 2013
 */

#include <types.h>
#include <arm/interrupt.h>
#include <arm/physmem.h>
#include <bits/fileno.h>
#include <bits/errno.h>
#include <exports.h>
#include <chars.h>

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
    if (!(buffer_start >= RAM_START_ADDR && buffer_end <= RAM_END_ADDR))
        return -EFAULT;
    
    // Read chars from stdin until buffer is full.
    while (read_count != count) {
        c = (uchar) getc();
        
        switch (c) {
        case CHAR_EOT:
            return read_count;
        case CHAR_BACKSPACE:
        case CHAR_DELETE:
            // Remove character from buffer.
            *buffer = 0;
            
            // Correct counters and offsets accordingly.
            if (read_count > 0) {
                buffer--;
                read_count--;
                puts("\b \b");
            }

            break;
        case CHAR_NEWLINE:
        case CHAR_CR:
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
    uint32_t write_count = 0;
    
    // Re-cast buf into char pointer.
    uchar *buffer = (uchar *) buf;
    
    // For readability
    uint32_t buffer_start = (uint32_t) buffer;
    uint32_t buffer_end = ((uint32_t) buffer) + count - 1; 
    
    // Sanity checks.
    if (fd != STDOUT_FILENO)
        return -EBADF;
    if (!((buffer_end <= FLASH_END_ADDR) ||
          (buffer_start >= RAM_START_ADDR &&
           buffer_end <= RAM_END_ADDR)))
        return -EFAULT;
    
    while (write_count != count) {
        putc(*buffer++);
        write_count++;
    }
    
    // Force a buffer flush
    puts(" \b");
    
    return write_count;
}
