/**
 * @file    syscalls.h
 *
 * @brief   Kernel system calls prototypes.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    03 Nov 2013
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

void exit(int status);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
unsigned long time();
void sleep(unsigned long millis);

#endif
