/*
 * syscalls.h: Kernel system calls.
 *
 * Authors: Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * Date:    10 Oct 2013
 */

#include <bits/swi.h>

#ifndef SYSCALLS_H
#define SYSCALLS_H
void __swi(EXIT_SWI) exit(int status); // Implemented in ASM
ssize_t __swi(READ_SWI) read(int fd, void *buf, size_t count);
ssize_t __swi(WRITE_SWI) write(int fd, const void *buf, size_t count);
#endif
