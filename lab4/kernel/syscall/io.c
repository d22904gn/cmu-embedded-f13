/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 * 
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    14 Nov 2013
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <arm/physmem.h>
#include <exports.h>

// Convenience typedefs
typedef unsigned char uchar;

// Special characters
#define CHAR_EOT        4u
#define CHAR_BACKSPACE  8u
#define CHAR_DELETE     127u
#define CHAR_NEWLINE    10u
#define CHAR_CR         13u

/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd __attribute__((unused)), 
                     void *buf __attribute__((unused)),
                     size_t count __attribute__((unused))) {
    // Temp local vars
    uchar c = 0;
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

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd  __attribute__((unused)),
                      const void *buf  __attribute__((unused)),
                      size_t count  __attribute__((unused))) {
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
          (buffer_start >= RAM_START_ADDR && buffer_end <= RAM_END_ADDR)))
        return -EFAULT;
    
    while (write_count != count) {
        putc(*buffer++);
        write_count++;
    }
    
    return write_count;
}

