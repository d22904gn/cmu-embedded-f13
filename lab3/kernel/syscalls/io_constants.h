/**
 * @file    io_contants.h
 *
 * @brief   Constants related to I/O.
 *
 * @authors Wee Loong Kuan <wkuan@andrew.cmu.edu>
 *          Chin Yang Oh <chinyano@andrew.cmu.edu>
 *          Jennifer Lee <jcl1@andrew.cmu.edu>
 * @date    03 Nov 2013
 */

#ifndef IO_CONSTANTS_H
#define IO_CONSTANTS_H

// Convenience typedefs
typedef unsigned char uchar;

// Memory ranges
#define RAM_START   0xa0000000u
#define RAM_END     0xa3ffffffu
#define ROM_END     0x00ffffffu

// Special characters
#define EOT         4u
#define BACKSPACE   8u
#define DELETE      127u
#define NEWLINE     10u
#define CR          13u

#endif
