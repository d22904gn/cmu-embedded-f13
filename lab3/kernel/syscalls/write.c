/**
 * @file    write.c
 *
 * @brief   Write() system call.
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
    if (!((buffer_end <= ROM_END) ||
          (buffer_start >= RAM_START && buffer_end <= RAM_END)))
        return -EFAULT;
    
    while (write_count != count) {
        putc(*buffer++);
        write_count++;
    }
    
    return write_count;
}
